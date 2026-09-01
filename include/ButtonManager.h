#pragma once

#include <Arduino.h>


enum class ButtonEvent : uint8_t
{
    None,
    Up,
    Down,
    Confirm,
    Back
};


class ButtonManager
{
public:
    ButtonManager();

    void begin();

    ButtonEvent update();


private:
    struct ButtonState
    {
        uint8_t pin;
        ButtonEvent event;

        bool stableState;
        bool lastReading;

        unsigned long lastChangeTime;
    };


    static constexpr size_t BUTTON_COUNT = 4;

    static constexpr unsigned long DEBOUNCE_TIME_MS = 25;


    ButtonState buttons[BUTTON_COUNT];
};