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

            {"~", {0x1E, KEY_RIGHT_ALT}},
            {"ˇ", {0x1F, KEY_RIGHT_ALT}},
            {"^", {0x20, KEY_RIGHT_ALT}},
            {"˘", {0x21, KEY_RIGHT_ALT}},
            {"°", {0x22, KEY_RIGHT_ALT}},
            {"˛", {0x23, KEY_RIGHT_ALT}},
            {"`", {0x24, KEY_RIGHT_ALT}},
            {"˙", {0x25, KEY_RIGHT_ALT}},
            {"´", {0x26, KEY_RIGHT_ALT}},
        };
};

#endif