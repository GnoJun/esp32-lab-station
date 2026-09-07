#include "ButtonManager.h"

#include "pins.h"


ButtonManager::ButtonManager()
    : buttons
    {
        {
            Pins::BUTTON_UP,
            ButtonEvent::Up,
            HIGH,
            HIGH,
            0
        },

        {
            Pins::BUTTON_DOWN,
            ButtonEvent::Down,
            HIGH,
            HIGH,
            0
        },

        {
            Pins::BUTTON_CONFIRM,
            ButtonEvent::Confirm,
            HIGH,
            HIGH,
            0
        },

        {
            Pins::BUTTON_BACK,
            ButtonEvent::Back,
            HIGH,
            HIGH,
            0
        }
    }
{
}


// --------------------------------------------------
// Begin
// --------------------------------------------------

void ButtonManager::begin()
{
    for (size_t i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(buttons[i].pin, INPUT_PULLUP);

        bool currentState =
            digitalRead(buttons[i].pin);

        buttons[i].stableState =
            currentState;

        buttons[i].lastReading =
            currentState;

        buttons[i].lastChangeTime =
            millis();
    }
}


// --------------------------------------------------
// Update
// --------------------------------------------------

ButtonEvent ButtonManager::update()
{
    unsigned long now = millis();

    ButtonEvent detectedEvent =
        ButtonEvent::None;


    for (size_t i = 0; i < BUTTON_COUNT; i++)
    {
        ButtonState& button =
            buttons[i];

        bool reading =
            digitalRead(button.pin);


        // Raw input changed
        if (reading != button.lastReading)
        {
            button.lastReading =
                reading;

            button.lastChangeTime =
                now;
        }


        // Has the new state remained stable long enough?
        if ((now - button.lastChangeTime)
            >= DEBOUNCE_TIME_MS)
        {
            if (reading != button.stableState)
            {
                button.stableState =
                    reading;


                // INPUT_PULLUP:
                // LOW means button pressed
                if (button.stableState == LOW)
                {
                    if (detectedEvent ==
                        ButtonEvent::None)
                    {
                        detectedEvent =
                            button.event;
                    }
                }
            }
        }
    }


    return detectedEvent;
}

bool ButtonManager::isHeld(
    ButtonEvent event
) const
{
    for (size_t i = 0; i < BUTTON_COUNT; i++)
    {
        if (buttons[i].event == event)
        {
            return buttons[i].stableState == LOW;
        }
    }

    return false;
}