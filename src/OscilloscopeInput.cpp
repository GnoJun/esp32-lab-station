#include "OscilloscopeInput.h"

#include "pins.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void OscilloscopeInput::begin()
{
    pinMode(
        Pins::SCOPE_IN,
        INPUT
    );


    // ESP32-S3 ADC:
    // 12-bit -> 0 to 4095
    analogReadResolution(12);


    // Configure only GPIO1.
    analogSetPinAttenuation(
        Pins::SCOPE_IN,
        ADC_11db
    );


    averageRaw = 0;

    averageMillivolts = 0;

    minRaw = 0;

    maxRaw = 0;
}


// --------------------------------------------------
// Sample
// --------------------------------------------------

void OscilloscopeInput::sample()
{
    uint32_t rawSum = 0;

    uint32_t millivoltSum = 0;


    uint16_t localMin = 4095;

    uint16_t localMax = 0;


    for (uint8_t i = 0;
         i < SAMPLE_COUNT;
         i++)
    {
        uint16_t raw =
            analogRead(
                Pins::SCOPE_IN
            );


        uint32_t millivolts =
            analogReadMilliVolts(
                Pins::SCOPE_IN
            );


        rawSum += raw;

        millivoltSum +=
            millivolts;


        if (raw < localMin)
        {
            localMin = raw;
        }


        if (raw > localMax)
        {
            localMax = raw;
        }
    }


    averageRaw =
        rawSum / SAMPLE_COUNT;


    averageMillivolts =
        millivoltSum / SAMPLE_COUNT;


    minRaw =
        localMin;


    maxRaw =
        localMax;
}


// --------------------------------------------------
// Getters
// --------------------------------------------------

uint16_t OscilloscopeInput::getAverageRaw() const
{
    return averageRaw;
}


uint32_t OscilloscopeInput::getAverageMillivolts() const
{
    return averageMillivolts;
}


uint16_t OscilloscopeInput::getMinRaw() const
{
    return minRaw;
}


uint16_t OscilloscopeInput::getMaxRaw() const
{
    return maxRaw;
}

uint32_t OscilloscopeInput::getInputMillivolts() const
{
    float calibrated =
        averageMillivolts *
        INPUT_CALIBRATION_GAIN;


    return static_cast<uint32_t>(
        calibrated + 0.5f
    );
}