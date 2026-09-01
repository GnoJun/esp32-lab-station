#pragma once

#include <Arduino.h>

namespace Pins
{
    // I2C
    constexpr uint8_t I2C_SDA = 8;
    constexpr uint8_t I2C_SCL = 9;

    // Buttons
    constexpr uint8_t BUTTON_UP      = 47;
    constexpr uint8_t BUTTON_DOWN    = 39;
    constexpr uint8_t BUTTON_CONFIRM = 40;
    constexpr uint8_t BUTTON_BACK    = 41;

    // Buzzer
    constexpr uint8_t BUZZER = 42;

    // Lab functions
    constexpr uint8_t SIGNAL_OUT = 4;
    constexpr uint8_t SCOPE_IN   = 1;
}