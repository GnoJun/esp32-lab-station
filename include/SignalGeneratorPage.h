#pragma once

#include <Arduino.h>

#include "ButtonManager.h"
#include "SignalGenerator.h"


enum class SignalPageResult : uint8_t
{
    None,
    Redraw,
    ExitRequested
};


class SignalGeneratorPage
{
public:
    void begin();

    SignalPageResult handleEvent(
        ButtonEvent event,
        SignalGenerator& generator
    );

    uint8_t getSelectedIndex() const;

    bool isEditing() const;

    SignalPageResult updateHold(
        const ButtonManager& buttons,
        SignalGenerator& generator
    );


private:
    static constexpr uint8_t FIELD_COUNT = 4;

    uint8_t selectedIndex = 0;

    bool editing = false;

    static constexpr unsigned long HOLD_DELAY_MS = 450;

    static constexpr unsigned long REPEAT_INTERVAL_MS = 120;


    ButtonEvent activeHoldButton =
        ButtonEvent::None;

    unsigned long holdStartTime = 0;

    unsigned long lastRepeatTime = 0;


    void resetHold();

    void applyAdjustment(
        ButtonEvent direction,
        SignalGenerator& generator
    );
};