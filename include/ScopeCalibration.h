#pragma once

#include <Arduino.h>

#include "esp_adc_cal.h"


class ScopeCalibration
{
public:
    void begin();

    uint32_t rawToAdcMillivolts(
        uint16_t raw
    ) const;

    uint32_t rawToInputMillivolts(
        uint16_t raw
    ) const;


    // Measured calibration from our actual
    // 20k / 20k scope front-end.
    static constexpr float INPUT_GAIN =
        2.04424f;


private:
    esp_adc_cal_characteristics_t
        adcCharacteristics = {};
};