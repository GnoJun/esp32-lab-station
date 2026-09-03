#pragma once

#include <Arduino.h>


class SignalGenerator
{
public:
    bool begin();

    bool increaseFrequency();
    bool decreaseFrequency();

    bool toggleOutput();

    uint32_t getFrequency() const;
    uint8_t getDutyPercent() const;
    bool isOutputEnabled() const;


private:
    static constexpr uint32_t MIN_FREQUENCY_HZ = 1;
    static constexpr uint32_t MAX_FREQUENCY_HZ = 20000;

    uint32_t frequencyHz = 1000;

    uint8_t dutyPercent = 50;

    bool outputEnabled = false;

    uint8_t resolutionBits = 13;


    bool applySettings();

    uint32_t getFrequencyStep() const;

    uint8_t chooseResolution(
        uint32_t frequency
    ) const;
};