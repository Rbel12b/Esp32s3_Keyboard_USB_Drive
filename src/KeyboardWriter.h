#pragma once
#ifndef _KEYBOARD_STRRAM_H_
#define _KEYBOARD_STRRAM_H_

#include "Arduino.h"

class KeyboardWriter
  : public Print
{
public:
    size_t write(uint8_t byte) {
        if (buffer.empty()) {
            expectedLength = utf8Length(byte);
        }
        buffer.push_back(static_cast<char>(byte));

        if (buffer.size() == expectedLength) {
            writeutf8(buffer);
            buffer.clear();
            expectedLength = 0;
        }
        return 1;
    }

private:
    std::string buffer;
    size_t expectedLength = 0;

    static size_t utf8Length(uint8_t firstByte) {
        if ((firstByte & 0x80) == 0x00) return 1;       // 0xxxxxxx
        if ((firstByte & 0xE0) == 0xC0) return 2;       // 110xxxxx
        if ((firstByte & 0xF0) == 0xE0) return 3;       // 1110xxxx
        if ((firstByte & 0xF8) == 0xF0) return 4;       // 11110xxx
        return 1; // fallback for invalid UTF-8
    }

    virtual void writeutf8(const std::string& utf8Char) = 0;
};

#endif