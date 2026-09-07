#pragma once

#include <Arduino.h>

#include "ScopeCalibration.h"


class OscilloscopeMeasurements
{
public:
    void begin();

    void update(
        uint16_t minRaw,
        uint16_t maxRaw,
        const ScopeCalibration& calibration
    );


    uint32_t getMinMillivolts() const;

    uint32_t getMaxMillivolts() const;

    uint32_t getPeakToPeakMillivolts() const;


private:
    uint32_t minMillivolts = 0;

    uint32_t maxMillivolts = 0;

    uint32_t peakToPeakMillivolts = 0;
};