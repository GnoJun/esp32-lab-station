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


private:
    static constexpr uint8_t FIELD_COUNT = 4;

    uint8_t selectedIndex = 0;

    bool editing = false;
};