#include "ScopeCalibration.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void ScopeCalibration::begin()
{
    esp_adc_cal_characterize(
        ADC_UNIT_1,
        ADC_ATTEN_DB_11,
        ADC_WIDTH_BIT_12,
        1100,
        &adcCharacteristics
    );
}


// --------------------------------------------------
// Raw -> ADC pin voltage
// --------------------------------------------------

uint32_t ScopeCalibration::rawToAdcMillivolts(
    uint16_t raw
) const
{
    return esp_adc_cal_raw_to_voltage(
        raw,
        &adcCharacteristics
    );
}


// --------------------------------------------------
// Raw -> actual Scope Input voltage
// --------------------------------------------------

uint32_t ScopeCalibration::rawToInputMillivolts(
    uint16_t raw
) const
{
    uint32_t adcMillivolts =
        rawToAdcMillivolts(raw);


    float inputMillivolts =
        static_cast<float>(
            adcMillivolts
        ) *
        INPUT_GAIN;


    return static_cast<uint32_t>(
        inputMillivolts + 0.5f
    );
}