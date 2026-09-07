#include "OscilloscopeMeasurements.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void OscilloscopeMeasurements::begin()
{
    minMillivolts = 0;

    maxMillivolts = 0;

    peakToPeakMillivolts = 0;
}


// --------------------------------------------------
// Update measurements
// --------------------------------------------------

void OscilloscopeMeasurements::update(
    uint16_t minRaw,
    uint16_t maxRaw,
    const ScopeCalibration& calibration
)
{
    minMillivolts =
        calibration.rawToInputMillivolts(
            minRaw
        );


    maxMillivolts =
        calibration.rawToInputMillivolts(
            maxRaw
        );


    if (maxMillivolts >= minMillivolts)
    {
        peakToPeakMillivolts =
            maxMillivolts -
            minMillivolts;
    }
    else
    {
        peakToPeakMillivolts = 0;
    }
}


// --------------------------------------------------
// Getters
// --------------------------------------------------

uint32_t OscilloscopeMeasurements::getMinMillivolts() const
{
    return minMillivolts;
}


uint32_t OscilloscopeMeasurements::getMaxMillivolts() const
{
    return maxMillivolts;
}


uint32_t OscilloscopeMeasurements::getPeakToPeakMillivolts() const
{
    return peakToPeakMillivolts;
}