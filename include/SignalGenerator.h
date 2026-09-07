#pragma once

#include <Arduino.h>


class SignalGenerator
{
public:
    bool begin();

    bool increaseFrequency();
    bool decreaseFrequency();

    bool increaseDuty();
    bool decreaseDuty();

    bool increaseStep();
    bool decreaseStep();

    bool toggleOutput();

    uint32_t getFrequency() const;
    uint8_t getDutyPercent() const;
    uint32_t getStepHz() const;
    bool isOutputEnabled() const;


private:
    static constexpr uint32_t MIN_FREQUENCY_HZ = 1;
    static constexpr uint32_t MAX_FREQUENCY_HZ = 20000;

    uint32_t frequencyHz = 1000;

    uint8_t dutyPercent = 50;

    uint32_t stepHz = 100;

    bool outputEnabled = false;

    uint8_t resolutionBits = 13;


    bool applySettings();


    uint8_t chooseResolution(
        uint32_t frequency
    ) const;
};