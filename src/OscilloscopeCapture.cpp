#include "OscilloscopeCapture.h"

#include "pins.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void OscilloscopeCapture::begin()
{
    for (uint16_t i = 0;
         i < SAMPLE_COUNT;
         i++)
    {
        samples[i] = 0;
    }


    minRaw = 0;

    maxRaw = 0;

    sampleIndex = 0;

    capturing = false;

    frameAvailable = false;
}


// --------------------------------------------------
// Timebase lookup
// --------------------------------------------------

uint32_t OscilloscopeCapture::intervalForIndex(
    uint8_t index
) const
{
    switch (index)
    {
        case 0:
            return 50;

        case 1:
            return 100;

        case 2:
            return 500;

        case 3:
            return 1000;

        case 4:
            return 5000;

        case 5:
            return 10000;

        default:
            return 100;
    }
}


// --------------------------------------------------
// Start capture
// --------------------------------------------------

void OscilloscopeCapture::startCapture()
{
    sampleIndex = 0;

    workingMinRaw = 4095;

    workingMaxRaw = 0;


    previousTriggerSampleValid =
        false;


    nextSampleTimeUs =
        micros();


    triggerWaitStartUs =
        micros();


    // If we already know the signal range,
    // wait for a rising edge first.
    waitingForTrigger =
        triggerReferenceValid;


    capturing = true;

    frameAvailable = false;
}


// --------------------------------------------------
// Non-blocking capture update
// --------------------------------------------------

bool OscilloscopeCapture::update()
{
    if (!capturing)
    {
        return false;
    }


    uint32_t now =
        micros();


    if (
        static_cast<int32_t>(
            now - nextSampleTimeUs
        ) < 0
    )
    {
        return false;
    }


    uint16_t raw =
        analogRead(
            Pins::SCOPE_IN
        );

    // ==================================================
    // Waiting for rising-edge trigger
    // ==================================================

    if (waitingForTrigger)
    {
        bool triggerFound =
            false;


        if (previousTriggerSampleValid)
        {
            if (
                previousTriggerRaw < triggerLevelRaw &&
                raw >= triggerLevelRaw
            )
            {
                triggerFound =
                    true;
            }
        }


        previousTriggerRaw =
            raw;

        previousTriggerSampleValid =
            true;


        bool triggerTimedOut =
            (
                static_cast<uint32_t>(
                    micros() - triggerWaitStartUs
                ) >=
                getTriggerTimeoutUs()
            );


        if (!triggerFound &&
            !triggerTimedOut)
        {
            nextSampleTimeUs +=
                getSampleIntervalUs();

            return false;
        }


        // Trigger found or AUTO timeout reached.
        waitingForTrigger =
            false;

        sampleIndex =
            0;

        workingMinRaw =
            4095;

        workingMaxRaw =
            0;


        storeSample(raw);


        nextSampleTimeUs +=
            getSampleIntervalUs();

        return false;
    }


    // ==================================================
    // Normal frame capture
    // ==================================================

    storeSample(raw);


    // --------------------------------------------------
    // Frame complete
    // --------------------------------------------------

    if (sampleIndex >= SAMPLE_COUNT)
    {
        minRaw =
            workingMinRaw;

        maxRaw =
            workingMaxRaw;

        uint16_t signalRange =
            maxRaw - minRaw;


        if (signalRange >= MIN_TRIGGER_RANGE_RAW)
        {
            triggerLevelRaw =
                minRaw +
                signalRange / 2;

            triggerReferenceValid =
                true;
        }
        else
        {
            triggerReferenceValid =
                false;
        }

        capturing =
            false;

        frameAvailable =
            true;


        return true;
    }


    nextSampleTimeUs +=
        getSampleIntervalUs();


    return false;
}


// --------------------------------------------------
// Faster Timebase
// --------------------------------------------------

bool OscilloscopeCapture::fasterTimebase()
{
    if (timebaseIndex == 0)
    {
        return false;
    }


    timebaseIndex--;

    startCapture();

    return true;
}


// --------------------------------------------------
// Slower Timebase
// --------------------------------------------------

bool OscilloscopeCapture::slowerTimebase()
{
    if (
        timebaseIndex >=
        TIMEBASE_COUNT - 1
    )
    {
        return false;
    }


    timebaseIndex++;

    startCapture();

    return true;
}


// --------------------------------------------------
// Status
// --------------------------------------------------

bool OscilloscopeCapture::isCapturing() const
{
    return capturing;
}


bool OscilloscopeCapture::hasFrame() const
{
    return frameAvailable;
}


// --------------------------------------------------
// Getters
// --------------------------------------------------

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
    return intervalForIndex(
        timebaseIndex
    );
}


uint32_t OscilloscopeCapture::getFrameDurationUs() const
{
    return
        getSampleIntervalUs() *
        SAMPLE_COUNT;
}

void OscilloscopeCapture::storeSample(
    uint16_t raw
)
{
    samples[sampleIndex] =
        raw;


    if (raw < workingMinRaw)
    {
        workingMinRaw =
            raw;
    }


    if (raw > workingMaxRaw)
    {
        workingMaxRaw =
            raw;
    }


    sampleIndex++;
}

uint32_t OscilloscopeCapture::getTriggerTimeoutUs() const
{
    uint32_t frameDuration =
        getFrameDurationUs();


    if (frameDuration < 100000)
    {
        return 100000;
    }


    if (frameDuration > 1500000)
    {
        return 1500000;
    }


    return frameDuration;
}