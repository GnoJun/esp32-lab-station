#pragma once

#include <Arduino.h>
#include "ButtonManager.h"


enum class AppPage : uint8_t
{
    // Main level
    MainMenu,

    // Main tools
    Oscilloscope,
    SignalGenerator,
    Tools,
    Settings,

    // Tools submenu
    TimerAlarmMenu,
    I2CScanner,
    Voltmeter,
    FrequencyMeter,
    DigitalInput,

    // Timer / Alarm submenu
    Timer,
    Alarm
};


class MenuManager
{
public:
    MenuManager();

    void begin();

    bool handleEvent(ButtonEvent event);

    uint8_t getSelectedIndex() const;
    uint8_t getToolsSelectedIndex() const;

    uint8_t getTimerAlarmSelectedIndex() const;

    AppPage getCurrentPage() const;


private:
    static constexpr uint8_t MAIN_MENU_ITEM_COUNT = 4;
    static constexpr uint8_t TOOLS_MENU_ITEM_COUNT = 5;
    static constexpr uint8_t TIMER_ALARM_ITEM_COUNT = 2;


    uint8_t selectedIndex;

    uint8_t toolsSelectedIndex;

    uint8_t timerAlarmSelectedIndex;


    AppPage currentPage;
};