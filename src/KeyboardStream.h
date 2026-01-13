#pragma once
#ifndef _KEYBOARD_STRRAM_H_
#define _KEYBOARD_STRRAM_H_

#include "Arduino.h"

class KeyboardWriter
  : public Print
{
public:
    size_t write(uint8_t)
    {
    }
};

#endif