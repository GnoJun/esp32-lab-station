#pragma once

#include <Arduino.h>


class OscilloscopeCapture
{
public:
    static constexpr uint16_t SAMPLE_COUNT = 128;

    void begin();

    void startCapture();

    bool update();

    bool fasterTimebase();

    bool slowerTimebase();

    bool isCapturing() const;

    bool hasFrame() const;

    uint16_t getSample(uint16_t index) const;

    uint16_t getMinRaw() const;

    uint16_t getMaxRaw() const;

    uint32_t getSampleIntervalUs() const;

    uint32_t getFrameDurationUs() const;

    float getMeasuredFrequencyHz() const;


private:
    static constexpr uint8_t TIMEBASE_COUNT = 6;

    uint16_t samples[SAMPLE_COUNT];

    uint16_t minRaw = 0;

    uint16_t maxRaw = 0;


    uint16_t workingMinRaw = 4095;

    uint16_t workingMaxRaw = 0;


    uint16_t sampleIndex = 0;


    // 0 = 50 us
    // 1 = 100 us
    // 2 = 500 us
    // 3 = 1 ms
    // 4 = 5 ms
    // 5 = 10 ms
    uint8_t timebaseIndex = 1;


    bool capturing = false;

    bool frameAvailable = false;


    uint32_t nextSampleTimeUs = 0;


    uint32_t intervalForIndex(
        uint8_t index
    ) const;

    bool waitingForTrigger = false;

    bool triggerReferenceValid = false;

    bool previousTriggerSampleValid = false;


    uint16_t triggerLevelRaw = 0;

    uint16_t previousTriggerRaw = 0;


    uint32_t triggerWaitStartUs = 0;


    static constexpr uint16_t MIN_TRIGGER_RANGE_RAW = 40;


    uint32_t getTriggerTimeoutUs() const;

    void storeSample(
        uint16_t raw
    );

    static constexpr uint32_t FAST_CAPTURE_MAX_INTERVAL_US = 100;


    bool frameStartedOnTrigger = false;

    float measuredFrequencyHz = 0.0f;


    bool captureFastRemainder();

    bool finishFrame();

    void calculateFrequency();
};