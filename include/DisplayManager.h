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


private:
    U8G2& mainDisplay;
    U8G2& menuDisplay;
};