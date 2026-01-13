#pragma once
#ifndef _KEYBOARD_HU_H_
#define _KEYBOARD_HU_H_

#include "KeyboardWriter.h"
#include "unordered_map"

class Keyboard_hu
    : public KeyboardWriter
{
protected:
    void writeutf8(const std::string &utf8Char) override
    {
        if (keyMap.find(utf8Char) != keyMap.end())
        {
            charInfo info = keyMap[utf8Char];
            KeyReport report;
            report.modifiers = info.mods;
            report.keys[0] = info.keyCode;
            for (int i = 1; i < 6; i++)
                report.keys[i] = 0;
            m_report(report);
        }
        // use utf8 input
        if (m_targetOS == 0)
        {

            // Linux - Ctrl + Shift + U + hex code + Enter
            m_report({KEY_LEFT_CTRL | KEY_LEFT_SHIFT, 0, {0x18, 0, 0, 0, 0, 0}}); // Ctrl + Shift + U
            delayMicroseconds(100);
            m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
            for (char c : utf8Char)
            {
                std::string s = std::to_string(c);
                if (s.length() == 1)
                    s = "0" + s;
                for (char digit : s)
                {
                    std::string charStr = "";
                    charStr += digit;
                    auto charInfo = keyMap[charStr];
                    KeyReport report;
                    report.modifiers = charInfo.mods;
                    report.keys[0] = charInfo.keyCode;
                    for (int i = 1; i < 6; i++)
                        report.keys[i] = 0;
                    m_report(report);
                    delayMicroseconds(100);
                    m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
                }
            }
            m_report({0, 0, {0x28, 0, 0, 0, 0, 0}}); // Enter
            delayMicroseconds(100);
            m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
        }
    }

private:
    struct charInfo
    {
        uint8_t keyCode;
        uint8_t mods;
    };

    std::unordered_map<std::string, charInfo> keyMap =
        {
            // Number row
            {"0", {0x35, 0}},
            {"1", {0x1E, 0}},
            {"2", {0x1F, 0}},
            {"3", {0x20, 0}},
            {"4", {0x21, 0}},
            {"5", {0x22, 0}},
            {"6", {0x23, 0}},
            {"7", {0x24, 0}},
            {"8", {0x25, 0}},
            {"9", {0x26, 0}},
            {"ö", {0x27, 0}},
            {"ü", {0x2D, 0}},
            {"ó", {0x2E, 0}},

            // Number row with Shift
            {"§", {0x35, KEY_LEFT_SHIFT}},
            {"'", {0x1E, KEY_LEFT_SHIFT}},
            {"\"", {0x1F, KEY_LEFT_SHIFT}},
            {"+", {0x20, KEY_LEFT_SHIFT}},
            {"!", {0x21, KEY_LEFT_SHIFT}},
            {"%", {0x22, KEY_LEFT_SHIFT}},
            {"/", {0x23, KEY_LEFT_SHIFT}},
            {"=", {0x24, KEY_LEFT_SHIFT}},
            {"(", {0x25, KEY_LEFT_SHIFT}},
            {")", {0x26, KEY_LEFT_SHIFT}},
            {"Ö", {0x27, KEY_LEFT_SHIFT}},
            {"Ü", {0x2D, KEY_LEFT_SHIFT}},
            {"Ó", {0x2E, KEY_LEFT_SHIFT}},

            // Number row with AltGr
            {"~", {0x1E, KEY_RIGHT_ALT}},
            {"ˇ", {0x1F, KEY_RIGHT_ALT}},
            {"^", {0x20, KEY_RIGHT_ALT}},
            {"˘", {0x21, KEY_RIGHT_ALT}},
            {"°", {0x22, KEY_RIGHT_ALT}},
            {"˛", {0x23, KEY_RIGHT_ALT}},
            {"`", {0x24, KEY_RIGHT_ALT}},
            {"˙", {0x25, KEY_RIGHT_ALT}},
            {"´", {0x26, KEY_RIGHT_ALT}},
            {"˝", {0x27, KEY_RIGHT_ALT}},
            {"¨", {0x2D, KEY_RIGHT_ALT}},
            {"¸", {0x2E, KEY_RIGHT_ALT}},

            // Q row
            {"q", {0x14, 0}},
            {"w", {0x1A, 0}},
            {"e", {0x08, 0}},
            {"r", {0x15, 0}},
            {"t", {0x17, 0}},
            {"z", {0x1C, 0}},
            {"u", {0x18, 0}},
            {"i", {0x0C, 0}},
            {"o", {0x12, 0}},
            {"p", {0x13, 0}},
            {"ő", {0x2F, 0}},
            {"ú", {0x30, 0}},
            {"ű", {0x31, 0}},

            // Q row with Shift
            {"Q", {0x14, KEY_LEFT_SHIFT}},
            {"W", {0x1A, KEY_LEFT_SHIFT}},
            {"E", {0x08, KEY_LEFT_SHIFT}},
            {"R", {0x15, KEY_LEFT_SHIFT}},
            {"T", {0x17, KEY_LEFT_SHIFT}},
            {"Z", {0x1C, KEY_LEFT_SHIFT}},
            {"U", {0x18, KEY_LEFT_SHIFT}},
            {"I", {0x0C, KEY_LEFT_SHIFT}},
            {"O", {0x12, KEY_LEFT_SHIFT}},
            {"P", {0x13, KEY_LEFT_SHIFT}},
            {"Ő", {0x2F, KEY_LEFT_SHIFT}},
            {"Ú", {0x30, KEY_LEFT_SHIFT}},
            {"Ű", {0x31, KEY_LEFT_SHIFT}},

            // Q row with AltGr
            {"\\", {0x14, KEY_RIGHT_ALT}},
            {"|", {0x1A, KEY_RIGHT_ALT}},
            {"Ä", {0x08, KEY_RIGHT_ALT}},
            {"€", {0x18, KEY_RIGHT_ALT}},
            {"Í", {0x0C, KEY_RIGHT_ALT}},
            {"÷", {0x2F, KEY_RIGHT_ALT}},
            {"×", {0x30, KEY_RIGHT_ALT}},
            {"¤", {0x31, KEY_RIGHT_ALT}},

            // A row
            {"a", {0x04, 0}},
            {"s", {0x16, 0}},
            {"d", {0x07, 0}},
            {"f", {0x09, 0}},
            {"g", {0x0A, 0}},
            {"h", {0x0B, 0}},
            {"j", {0x0D, 0}},
            {"k", {0x0E, 0}},
            {"l", {0x0F, 0}},
            {"é", {0x31, 0}},
            {"á", {0x2E, 0}},

            // A row with Shift
            {"A", {0x04, KEY_LEFT_SHIFT}},
            {"S", {0x16, KEY_LEFT_SHIFT}},
            {"D", {0x07, KEY_LEFT_SHIFT}},
            {"F", {0x09, KEY_LEFT_SHIFT}},
            {"G", {0x0A, KEY_LEFT_SHIFT}},
            {"H", {0x0B, KEY_LEFT_SHIFT}},
            {"J", {0x0D, KEY_LEFT_SHIFT}},
            {"K", {0x0E, KEY_LEFT_SHIFT}},
            {"L", {0x0F, KEY_LEFT_SHIFT}},
            {"É", {0x31, KEY_LEFT_SHIFT}},
            {"Á", {0x2E, KEY_LEFT_SHIFT}},

            // A row with AltGr
            {"ä", {0x04, KEY_RIGHT_ALT}},
            {"đ", {0x16, KEY_RIGHT_ALT}},
            {"Đ", {0x07, KEY_RIGHT_ALT}},
            {"[", {0x09, KEY_RIGHT_ALT}},
            {"]", {0x0A, KEY_RIGHT_ALT}},
            {"í", {0x0D, KEY_RIGHT_ALT}},
            {"ł", {0x0E, KEY_RIGHT_ALT}},
            {"Ł", {0x0F, KEY_RIGHT_ALT}},
            {"$", {0x31, KEY_RIGHT_ALT}},
            {"ß", {0x2E, KEY_RIGHT_ALT}},

            // bottom row
            {"í", {0x64, 0}},
            {"y", {0x1D, 0}},
            {"x", {0x1B, 0}},
            {"c", {0x06, 0}},
            {"v", {0x19, 0}},
            {"b", {0x05, 0}},
            {"n", {0x11, 0}},
            {"m", {0x10, 0}},
            {",", {0x36, 0}},
            {".", {0x37, 0}},
            {"-", {0x38, 0}},

            // bottom row with Shift
            {"Í", {0x64, KEY_LEFT_SHIFT}},
            {"Y", {0x1D, KEY_LEFT_SHIFT}},
            {"X", {0x1B, KEY_LEFT_SHIFT}},
            {"C", {0x06, KEY_LEFT_SHIFT}},
            {"V", {0x19, KEY_LEFT_SHIFT}},
            {"B", {0x05, KEY_LEFT_SHIFT}},
            {"N", {0x11, KEY_LEFT_SHIFT}},
            {"M", {0x10, KEY_LEFT_SHIFT}},
            {"?", {0x36, KEY_LEFT_SHIFT}},
            {":", {0x37, KEY_LEFT_SHIFT}},
            {"_", {0x38, KEY_LEFT_SHIFT}},

            // bottom row with AltGr
            {"<", {0x64, KEY_RIGHT_ALT}},
            {">", {0x1D, KEY_RIGHT_ALT}},
            {"#", {0x1B, KEY_RIGHT_ALT}},
            {"&", {0x06, KEY_RIGHT_ALT}},
            {"@", {0x19, KEY_RIGHT_ALT}},
            {"{", {0x05, KEY_RIGHT_ALT}},
            {"}", {0x11, KEY_RIGHT_ALT}},
            {";", {0x36, KEY_RIGHT_ALT}},
            {"*", {0x38, KEY_RIGHT_ALT}},

            {" ", {0x2C, 0}} // space
        };
};

#endif