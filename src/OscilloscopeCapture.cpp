#include "OscilloscopeCapture.h"

#include "pins.h"


void OscilloscopeCapture::begin()
{
    for (uint16_t i = 0; i < SAMPLE_COUNT; i++)
    {
        samples[i] = 0;
    }

    minRaw = 0;
    maxRaw = 0;
}


void OscilloscopeCapture::capture()
{
    uint16_t localMin = 4095;
    uint16_t localMax = 0;


    uint32_t nextSampleTime =
        micros();


    for (uint16_t i = 0;
         i < SAMPLE_COUNT;
         i++)
    {
        while (
            static_cast<int32_t>(
                micros() - nextSampleTime
            ) < 0
        )
        {
            // Wait for exact sample time
        }


        uint16_t raw =
            analogRead(
                Pins::SCOPE_IN
            );


        samples[i] =
            raw;


        if (raw < localMin)
        {
            localMin = raw;
        }


        if (raw > localMax)
        {
            localMax = raw;
        }


        nextSampleTime +=
            SAMPLE_INTERVAL_US;
    }


    minRaw =
        localMin;

    maxRaw =
        localMax;
}


uint16_t OscilloscopeCapture::getSample(
    uint16_t index
) const
{
    if (index >= SAMPLE_COUNT)
    {
        return 0;
    }

    return samples[index];
}


uint16_t OscilloscopeCapture::getMinRaw() const
{
    return minRaw;
}


uint16_t OscilloscopeCapture::getMaxRaw() const
{
    return maxRaw;
}


uint32_t OscilloscopeCapture::getSampleIntervalUs() const
{
    return SAMPLE_INTERVAL_US;
}