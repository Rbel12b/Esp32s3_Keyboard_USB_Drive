#pragma once
#ifndef _KEYBOARD_HU_H_
#define _KEYBOARD_HU_H_

#include "KeyboardWriter.h"
#include "unordered_map"

#define MOD_ALTGR MOD_RALT

class Keyboard_hu
    : public KeyboardWriter
{
public:
    Keyboard_hu(std::function<void(KeyReport)> report, int targetOS = 0)
        : KeyboardWriter(report, targetOS)
    {
    }

protected:
    void writeutf8(const std::string &utf8Char) override
    {
        if (keyMap.count(utf8Char))
        {
            charInfo info = keyMap[utf8Char];
            KeyReport report;
            report.modifiers = info.mods;
            report.keys[0] = info.keyCode;
            for (int i = 1; i < 6; i++)
                report.keys[i] = 0;
            m_report(report);
            delayMicroseconds(100);
            m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
            return;
        }
        // use utf8 input
        // if (m_targetOS == 0)
        // {

        //     // Linux - Ctrl + Shift + U + hex code + Enter
        //     m_report({KEY_LEFT_CTRL | MOD_SHIFT, 0, {0x18, 0, 0, 0, 0, 0}}); // Ctrl + Shift + U
        //     delayMicroseconds(100);
        //     m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
        //     for (char c : utf8Char)
        //     {
        //         std::string s = std::to_string(c);
        //         if (s.length() == 1)
        //             s = "0" + s;
        //         for (char digit : s)
        //         {
        //             std::string charStr = "";
        //             charStr += digit;
        //             auto charInfo = keyMap[charStr];
        //             KeyReport report;
        //             report.modifiers = charInfo.mods;
        //             report.keys[0] = charInfo.keyCode;
        //             for (int i = 1; i < 6; i++)
        //                 report.keys[i] = 0;
        //             m_report(report);
        //             delayMicroseconds(100);
        //             m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
        //         }
        //     }
        //     m_report({0, 0, {0x28, 0, 0, 0, 0, 0}}); // Enter
        //     delayMicroseconds(100);
        //     m_report({0, 0, {0, 0, 0, 0, 0, 0}}); // release
        // }
    }

public:
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
            {"§", {0x35, MOD_SHIFT}},
            {"'", {0x1E, MOD_SHIFT}},
            {"\"", {0x1F, MOD_SHIFT}},
            {"+", {0x20, MOD_SHIFT}},
            {"!", {0x21, MOD_SHIFT}},
            {"%", {0x22, MOD_SHIFT}},
            {"/", {0x23, MOD_SHIFT}},
            {"=", {0x24, MOD_SHIFT}},
            {"(", {0x25, MOD_SHIFT}},
            {")", {0x26, MOD_SHIFT}},
            {"Ö", {0x27, MOD_SHIFT}},
            {"Ü", {0x2D, MOD_SHIFT}},
            {"Ó", {0x2E, MOD_SHIFT}},

            // Number row with AltGr
            {"~", {0x1E, MOD_ALTGR}},
            {"ˇ", {0x1F, MOD_ALTGR}},
            {"^", {0x20, MOD_ALTGR}},
            {"˘", {0x21, MOD_ALTGR}},
            {"°", {0x22, MOD_ALTGR}},
            {"˛", {0x23, MOD_ALTGR}},
            {"`", {0x24, MOD_ALTGR}},
            {"˙", {0x25, MOD_ALTGR}},
            {"´", {0x26, MOD_ALTGR}},
            {"˝", {0x27, MOD_ALTGR}},
            {"¨", {0x2D, MOD_ALTGR}},
            {"¸", {0x2E, MOD_ALTGR}},

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
            {"Q", {0x14, MOD_SHIFT}},
            {"W", {0x1A, MOD_SHIFT}},
            {"E", {0x08, MOD_SHIFT}},
            {"R", {0x15, MOD_SHIFT}},
            {"T", {0x17, MOD_SHIFT}},
            {"Z", {0x1C, MOD_SHIFT}},
            {"U", {0x18, MOD_SHIFT}},
            {"I", {0x0C, MOD_SHIFT}},
            {"O", {0x12, MOD_SHIFT}},
            {"P", {0x13, MOD_SHIFT}},
            {"Ő", {0x2F, MOD_SHIFT}},
            {"Ú", {0x30, MOD_SHIFT}},
            {"Ű", {0x31, MOD_SHIFT}},

            // Q row with AltGr
            {"\\", {0x14, MOD_ALTGR}},
            {"|", {0x1A, MOD_ALTGR}},
            {"Ä", {0x08, MOD_ALTGR}},
            {"€", {0x18, MOD_ALTGR}},
            {"Í", {0x0C, MOD_ALTGR}},
            {"÷", {0x2F, MOD_ALTGR}},
            {"×", {0x30, MOD_ALTGR}},
            {"¤", {0x31, MOD_ALTGR}},

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
            {"á", {0x34, 0}},

            // A row with Shift
            {"A", {0x04, MOD_SHIFT}},
            {"S", {0x16, MOD_SHIFT}},
            {"D", {0x07, MOD_SHIFT}},
            {"F", {0x09, MOD_SHIFT}},
            {"G", {0x0A, MOD_SHIFT}},
            {"H", {0x0B, MOD_SHIFT}},
            {"J", {0x0D, MOD_SHIFT}},
            {"K", {0x0E, MOD_SHIFT}},
            {"L", {0x0F, MOD_SHIFT}},
            {"É", {0x31, MOD_SHIFT}},
            {"Á", {0x2E, MOD_SHIFT}},

            // A row with AltGr
            {"ä", {0x04, MOD_ALTGR}},
            {"đ", {0x16, MOD_ALTGR}},
            {"Đ", {0x07, MOD_ALTGR}},
            {"[", {0x09, MOD_ALTGR}},
            {"]", {0x0A, MOD_ALTGR}},
            {"í", {0x0D, MOD_ALTGR}},
            {"ł", {0x0E, MOD_ALTGR}},
            {"Ł", {0x0F, MOD_ALTGR}},
            {"$", {0x31, MOD_ALTGR}},
            {"ß", {0x2E, MOD_ALTGR}},

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
            {"Í", {0x64, MOD_SHIFT}},
            {"Y", {0x1D, MOD_SHIFT}},
            {"X", {0x1B, MOD_SHIFT}},
            {"C", {0x06, MOD_SHIFT}},
            {"V", {0x19, MOD_SHIFT}},
            {"B", {0x05, MOD_SHIFT}},
            {"N", {0x11, MOD_SHIFT}},
            {"M", {0x10, MOD_SHIFT}},
            {"?", {0x36, MOD_SHIFT}},
            {":", {0x37, MOD_SHIFT}},
            {"_", {0x38, MOD_SHIFT}},

            // bottom row with AltGr
            {"<", {0x64, MOD_ALTGR}},
            {">", {0x1D, MOD_ALTGR}},
            {"#", {0x1B, MOD_ALTGR}},
            {"&", {0x06, MOD_ALTGR}},
            {"@", {0x19, MOD_ALTGR}},
            {"{", {0x05, MOD_ALTGR}},
            {"}", {0x11, MOD_ALTGR}},
            {";", {0x36, MOD_ALTGR}},
            {"*", {0x38, MOD_ALTGR}},

            {" ", {0x2C, 0}}, // space
            {"\n", {0x28, 0}} // enter
        };
};

#endif