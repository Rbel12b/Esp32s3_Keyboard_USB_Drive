#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBMSC.h"

#include <LittleFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <map>

#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include <SPI.h>

/* ---------- Globals ---------- */

USBHIDKeyboard Keyboard;
USBMSC MSC;
AsyncWebServer server(80);

sdmmc_card_t *sdcard;

/* ---------- SD RAW INIT ---------- */

#define SD_MISO GPIO_NUM_14 // MISO
#define SD_MOSI GPIO_NUM_12 // MOSI
#define SD_SCK GPIO_NUM_13  // SCK
#define SD_CS GPIO_NUM_11   // Chip select

void initSD()
{
    // 1) Configure SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096};

    // Initialize the SPI bus (esp-idf)
    esp_err_t ret = spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        Serial.printf("SPI bus initialize failed: %d\n", ret);
        return;
    }

    // 2) Attach SD card via SDSPI
    sdspi_device_config_t devcfg = SDSPI_DEVICE_CONFIG_DEFAULT();
    devcfg.gpio_cs = SD_CS;
    devcfg.host_id = SPI3_HOST;

    sdspi_dev_handle_t spi_handle;
    ret = sdspi_host_init_device(&devcfg, &spi_handle);
    if (ret != ESP_OK)
    {
        Serial.printf("sdspi_host_init_device failed: %d\n", ret);
        return;
    }

    // 4) Set up the SDMMC host struct for SPI mode
    static sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = (int)spi_handle; // **Use the handle returned above**

    // 5) Finally call sdmmc_card_init to detect the card
    sdcard = (sdmmc_card_t *)malloc(sizeof(sdmmc_card_t));
    ret = sdmmc_card_init(&host, sdcard);

    if (ret != ESP_OK)
    {
        Serial.printf("sdmmc_card_init failed: 0x%x\n", ret);
        free(sdcard);
        sdcard = nullptr;
        return;
    }

    Serial.println("SD card initialized (SPI mode)");
    sdmmc_card_print_info(stdout, sdcard);
}

/* ---------- MSC callbacks ---------- */

int32_t msc_read(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
    if (offset != 0) return -1;

    uint32_t sector_size = sdcard->csd.sector_size;
    if (bufsize % sector_size != 0) return -1;

    uint32_t count = bufsize / sector_size;

    esp_err_t err = sdmmc_read_sectors(sdcard, buffer, lba, count);
    return (err == ESP_OK) ? bufsize : -1;
}

int32_t msc_write(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
    if (offset != 0) return -1;

    uint32_t sector_size = sdcard->csd.sector_size;
    if (bufsize % sector_size != 0) return -1;

    uint32_t count = bufsize / sector_size;

    esp_err_t err = sdmmc_write_sectors(sdcard, buffer, lba, count);
    return (err == ESP_OK) ? bufsize : -1;
}


void msc_flush(void) {}

/* ---------- HID helpers ---------- */


/**
 * @brief Handle incoming request body
 * @param body Full body as string, formatted like "<keys-in-hex>|<mods-in-hex>"
 */
void report(const std::string &body)
{
    size_t sep = body.find('|');
    if (sep == std::string::npos) {
        log_e("Invalid report format");
        return;
    }

    std::string keys_str = body.substr(0, sep);
    std::string mods_str = body.substr(sep + 1);

    // Parse keys
    uint8_t keys[6] = {0};
    size_t key_count = 0;
    size_t pos = 0;
    while (pos < keys_str.length() && key_count < 6) {
        size_t next_pos = keys_str.find(',', pos);
        std::string key_hex = (next_pos == std::string::npos) ? 
                              keys_str.substr(pos) : 
                              keys_str.substr(pos, next_pos - pos);
        keys[key_count++] = (uint8_t)strtol(key_hex.c_str(), nullptr, 16);
        if (next_pos == std::string::npos) break;
        pos = next_pos + 1;
    }

    // Parse mods
    uint8_t mods = (uint8_t)strtol(mods_str.c_str(), nullptr, 16);

    // Prepare and send report
    KeyReport keysreport;
    keysreport.modifiers = mods;
    memcpy(keysreport.keys, keys, 6);
    Keyboard.sendReport(&keysreport);
}

static std::map<AsyncWebServerRequest*, std::string> bodyMap;

void onRequestBody(
    AsyncWebServerRequest *request,
    uint8_t *data,
    size_t len,
    size_t index,
    size_t total)
{
    // Append to the request-specific buffer
    std::string &body = bodyMap[request];

    // Reserve some space but cap it to reasonable size
    // to avoid huge allocations if total is bogus.
    if (index == 0) {
        size_t cap = (total > 0 && total < 4096) ? total : 512;
        body.reserve(cap);
    }

    body.append((const char*)data, len);

    // Not finished yet
    if (index + len != total) {
        return;
    }

    // At this point, we have the full body
    log_i("%s", body.c_str());

    if (request->url() == "/report") {
        report(body);
    }

    request->send(200, "text/plain", "true");

    // Remove request from map
    bodyMap.erase(request);
}

void setup()
{
    heap_caps_check_integrity_all(true);
    Serial.begin(115200);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    /* Flash FS for web UI */
    LittleFS.begin(true);

    /* RAW SD */
    initSD();

    MSC.onRead([](uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
      return msc_read(lba, offset, buffer, bufsize);
    });
    MSC.onWrite([](uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
      return msc_write(lba, offset, buffer, bufsize);
    });
    MSC.vendorID("ESPRESSIF");
    MSC.productID("ESP32 SD Card");
    MSC.productRevision("1.0");
    MSC.mediaPresent(true);
    MSC.begin(sdcard->csd.capacity, sdcard->csd.sector_size);

    /* USB HID */
    Keyboard.begin();
    USB.begin();

    vTaskDelay(1);

    log_i("USB initialized.");

    /* Wi-Fi UI */
    WiFi.softAP("ESP32-Keyboard");

    log_i("AP IP address: %s", WiFi.softAPIP().toString().c_str());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *r)
        { r->send(LittleFS, "/index.html", "text/html"); });

    server.onRequestBody(onRequestBody);

    // other files are served from littleFS automatically
    server.serveStatic("/", LittleFS, "/");

    log_i("Web server setup done.");

    vTaskDelay(1);

    server.begin();

    log_i("Web server started.");
}

void loop()
{
    vTaskDelay(100 / portTICK_PERIOD_MS);
    heap_caps_check_integrity_all(true);
}