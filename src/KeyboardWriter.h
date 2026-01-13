#pragma once
#ifndef _KEYBOARD_STRRAM_H_
#define _KEYBOARD_STRRAM_H_

#include "Arduino.h"
#include "string"
#include "USBHIDKeyboard.h"

#define MOD_RALT 0x40
#define MOD_SHIFT 0x02

class KeyboardWriter
    : public Print
{
public:
    /**
     * @brief Construct a new Keyboard Writer object
     * @param report Function to send KeyReport to USB HID
     * @param targetOS Target OS unicode input method (0 -> linux, X11, using ctrl+shift+u, 1 -> Windows, using U+XXXXXX alt+x)
     */
    KeyboardWriter(std::function<void(KeyReport)> report, int targetOS = 0)
        : m_report(report), m_targetOS(targetOS)
    {
    }

    size_t write(uint8_t byte) override
    {
        if (buffer.empty())
        {
            expectedLength = utf8Length(byte);
        }
        buffer.push_back(static_cast<char>(byte));

        if (buffer.size() == expectedLength)
        {
            writeutf8(buffer);
            buffer.clear();
            expectedLength = 0;
        }
        return 1;
    }

private:
    std::string buffer;
    size_t expectedLength = 0;

protected:
    std::function<void(KeyReport)> m_report;
    int m_targetOS;

    static size_t utf8Length(uint8_t firstByte)
    {
        if ((firstByte & 0x80) == 0x00)
            return 1; // 0xxxxxxx
        if ((firstByte & 0xE0) == 0xC0)
            return 2; // 110xxxxx
        if ((firstByte & 0xF0) == 0xE0)
            return 3; // 1110xxxx
        if ((firstByte & 0xF8) == 0xF0)
            return 4; // 11110xxx
        return 1;     // fallback for invalid UTF-8
    }

    virtual void writeutf8(const std::string &utf8Char) = 0;
};

#endif