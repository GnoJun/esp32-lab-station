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

    frameStartedOnTrigger = false;

    measuredFrequencyHz = 0.0f;
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
            return 100;

        case 1:
            return 500;

        case 2:
            return 1000;

        case 3:
            return 5000;

        case 4:
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

    frameStartedOnTrigger =
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


    bool fastTriggerSearch =
        waitingForTrigger &&
        (
            getSampleIntervalUs() <=
            FAST_CAPTURE_MAX_INTERVAL_US
        );


    if (!fastTriggerSearch)
    {
        if (
            static_cast<int32_t>(
                now - nextSampleTimeUs
            ) < 0
        )
        {
            return false;
        }
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
            if (
                getSampleIntervalUs() >
                FAST_CAPTURE_MAX_INTERVAL_US
            )
            {
                nextSampleTimeUs +=
                    getSampleIntervalUs();
            }


            return false;
        }


        // Trigger found or AUTO timeout reached.
        waitingForTrigger =
            false;


        frameStartedOnTrigger =
            triggerFound;


        sampleIndex =
            0;

        workingMinRaw =
            4095;

        workingMaxRaw =
            0;


        storeSample(raw);

        if (
            sampleIndex == 1 &&
            getSampleIntervalUs() <=
                FAST_CAPTURE_MAX_INTERVAL_US
        )
        {
            nextSampleTimeUs =
                micros() +
                getSampleIntervalUs();


            return captureFastRemainder();
        }


        nextSampleTimeUs =
            micros() +
            getSampleIntervalUs();


        // Fast timebases use a tightly timed capture.
        if (
            getSampleIntervalUs() <=
            FAST_CAPTURE_MAX_INTERVAL_US
        )
        {
            return captureFastRemainder();
        }


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
        return finishFrame();
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

bool OscilloscopeCapture::finishFrame()
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


    calculateFrequency();

    capturing =
        false;

    frameAvailable =
        true;


    return true;
}

bool OscilloscopeCapture::captureFastRemainder()
{
    uint32_t intervalUs =
        getSampleIntervalUs();


    while (sampleIndex < SAMPLE_COUNT)
    {
        while (
            static_cast<int32_t>(
                micros() - nextSampleTimeUs
            ) < 0
        )
        {
            // Tight timing wait.
        }


        uint16_t raw =
            analogRead(
                Pins::SCOPE_IN
            );


        storeSample(raw);


        nextSampleTimeUs +=
            intervalUs;
    }


    return finishFrame();
}

// --------------------------------------------------
// Calculate Frequency
// --------------------------------------------------

void OscilloscopeCapture::calculateFrequency()
{
    measuredFrequencyHz =
        0.0f;


    uint16_t signalRange =
        maxRaw - minRaw;


    // Signal too small to measure reliably.
    if (signalRange < MIN_TRIGGER_RANGE_RAW)
    {
        return;
    }


    // --------------------------------------------------
    // Add hysteresis around the middle of the waveform.
    //
    // Low threshold  = 40%
    // High threshold = 60%
    //
    // This prevents ADC noise around 50% from being
    // counted as multiple rising edges.
    // --------------------------------------------------

    uint16_t lowThreshold =
        minRaw +
        (
            static_cast<uint32_t>(
                signalRange
            ) * 4
        ) / 10;


    uint16_t highThreshold =
        minRaw +
        (
            static_cast<uint32_t>(
                signalRange
            ) * 6
        ) / 10;


    int16_t firstEdge =
        -1;

    int16_t lastEdge =
        -1;

    uint16_t edgeCount =
        0;


    // --------------------------------------------------
    // If the frame really started from our trigger,
    // sample 0 represents the first rising edge.
    // --------------------------------------------------

    if (frameStartedOnTrigger)
    {
        firstEdge = 0;

        lastEdge = 0;

        edgeCount = 1;
    }


    // The detector must first see a LOW level
    // before it is armed for the next rising edge.
    bool armed =
        samples[0] <= lowThreshold;


    // --------------------------------------------------
    // Find rising edges
    // --------------------------------------------------

    for (
        uint16_t i = 1;
        i < SAMPLE_COUNT;
        i++
    )
    {
        // Wait until signal goes sufficiently LOW.
        if (!armed)
        {
            if (samples[i] <= lowThreshold)
            {
                armed = true;
            }

            continue;
        }


        // Armed + sufficiently HIGH = rising edge.
        if (samples[i] >= highThreshold)
        {
            if (firstEdge < 0)
            {
                firstEdge =
                    static_cast<int16_t>(i);
            }


            lastEdge =
                static_cast<int16_t>(i);


            edgeCount++;


            // Must return LOW before another
            // edge may be counted.
            armed = false;
        }
    }


    // We need at least two rising edges
    // to measure one complete period.
    if (
        edgeCount < 2 ||
        firstEdge < 0 ||
        lastEdge <= firstEdge
    )
    {
        return;
    }


    // --------------------------------------------------
    // Average several periods when possible.
    // --------------------------------------------------

    float periodCount =
        static_cast<float>(
            edgeCount - 1
        );


    float sampleSpan =
        static_cast<float>(
            lastEdge - firstEdge
        );


    float averagePeriodSamples =
        sampleSpan /
        periodCount;


    // Too few samples per cycle gives unreliable
    // frequency / aliasing.
    if (averagePeriodSamples < 4.0f)
    {
        return;
    }


    float periodUs =
        averagePeriodSamples *
        static_cast<float>(
            getSampleIntervalUs()
        );


    if (periodUs <= 0.0f)
    {
        return;
    }


    measuredFrequencyHz =
        1000000.0f /
        periodUs;
}

float OscilloscopeCapture::getMeasuredFrequencyHz() const
{
    return measuredFrequencyHz;
}