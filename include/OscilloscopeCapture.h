#pragma once

#include <Arduino.h>


class OscilloscopeCapture
{
public:
    static constexpr uint16_t SAMPLE_COUNT = 128;

    void begin();

    void capture();

    uint16_t getSample(uint16_t index) const;

    uint16_t getMinRaw() const;

    uint16_t getMaxRaw() const;

    uint32_t getSampleIntervalUs() const;


private:
    static constexpr uint32_t SAMPLE_INTERVAL_US = 100;

    uint16_t samples[SAMPLE_COUNT];

    uint16_t minRaw = 0;

    uint16_t maxRaw = 0;
};