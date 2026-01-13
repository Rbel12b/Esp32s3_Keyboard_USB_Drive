#pragma once
#ifndef _KEYBOARD_HU_H_
#define _KEYBOARD_HU_H_

#include "KeyboardWriter.h"

class Keyboard_hu
  : public KeyboardWriter
{
    void writeutf8(const std::string& utf8Char)  override
    {
    }
};

#endif