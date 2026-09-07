#pragma once

#include <Arduino.h>


class OscilloscopeInput
{
public:
    void begin();

    void sample();

    uint16_t getAverageRaw() const;

    uint32_t getAverageMillivolts() const;

    uint16_t getMinRaw() const;

    uint16_t getMaxRaw() const;


private:
    static constexpr uint8_t SAMPLE_COUNT = 32;

    uint16_t averageRaw = 0;

    uint32_t averageMillivolts = 0;

    uint16_t minRaw = 0;

    uint16_t maxRaw = 0;
};