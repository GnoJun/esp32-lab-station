#pragma once

#include <Arduino.h>
#include <U8g2lib.h>


class DisplayManager
{
public:
    DisplayManager(
        U8G2& mainDisplay,
        U8G2& menuDisplay
    );

    void begin();

    void showBootScreen();

    void showMainMenu(
        uint8_t selectedIndex
    );

    void showSubMenu(
        const char* title,
        const char* const items[],
        uint8_t itemCount,
        uint8_t selectedIndex
    );

    void showToolPage(
        const char* title
    );

    void showI2CScanner(
        const uint8_t addresses[],
        uint8_t deviceCount
    );

    void showSignalGenerator(
        uint32_t frequencyHz,
        uint8_t dutyPercent,
        uint32_t stepHz,
        bool outputEnabled,
        uint8_t selectedIndex,
        bool editing
    );

    void showOscilloscopeBaseline(
        uint16_t averageRaw,
        uint32_t adcMillivolts,
        uint32_t inputMillivolts,
        uint16_t minRaw,
        uint16_t maxRaw
    );

    void showOscilloscopeWaveform(
        const uint16_t samples[],
        uint16_t sampleCount,
        uint16_t minRaw,
        uint16_t maxRaw,
        uint32_t sampleIntervalUs
    );


private:
    U8G2& mainDisplay;
    U8G2& menuDisplay;
};