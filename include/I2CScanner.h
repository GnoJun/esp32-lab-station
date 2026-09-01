#pragma once

#include <Arduino.h>
#include <Wire.h>


class I2CScanner
{
public:
    void begin();

    void scan();

    uint8_t getDeviceCount() const;

    uint8_t getAddress(uint8_t index) const;


private:
    static constexpr uint8_t MAX_DEVICES = 16;

    uint8_t addresses[MAX_DEVICES];

    uint8_t deviceCount = 0;
};