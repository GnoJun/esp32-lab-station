#include "SignalGeneratorPage.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void SignalGeneratorPage::begin()
{
    selectedIndex = 0;

    editing = false;

    resetHold();
}


// --------------------------------------------------
// Handle Event
// --------------------------------------------------

SignalPageResult SignalGeneratorPage::handleEvent(
    ButtonEvent event,
    SignalGenerator& generator
)
{
    // ==================================================
    // Navigation Mode
    // ==================================================

    if (!editing)
    {
        switch (event)
        {
            case ButtonEvent::Up:

            if (selectedIndex == 0)
            {
                selectedIndex =
                    FIELD_COUNT - 1;
            }
            else
            {
                selectedIndex--;
            }

            return SignalPageResult::Redraw;


            case ButtonEvent::Down:

            selectedIndex++;

            if (selectedIndex >= FIELD_COUNT)
            {
                selectedIndex = 0;
            }

            return SignalPageResult::Redraw;



            case ButtonEvent::Confirm:

                // Output is an immediate toggle.
                if (selectedIndex == 3)
                {
                    generator.toggleOutput();

                    return SignalPageResult::Redraw;
                }


                // Frequency, Duty and Step enter edit mode.
                editing = true;

                return SignalPageResult::Redraw;


            case ButtonEvent::Back:

                return SignalPageResult::ExitRequested;


            case ButtonEvent::None:

                return SignalPageResult::None;
        }
    }


    // ==================================================
    // Edit Mode
    // ==================================================

    switch (event)
    {
        case ButtonEvent::Up:

            switch (selectedIndex)
            {
                case 0:
                    generator.increaseFrequency();
                    break;

                case 1:
                    generator.increaseDuty();
                    break;

                case 2:
                    generator.increaseStep();
                    break;
            }

            return SignalPageResult::Redraw;


        case ButtonEvent::Down:

            switch (selectedIndex)
            {
                case 0:
                    generator.decreaseFrequency();
                    break;

                case 1:
                    generator.decreaseDuty();
                    break;

                case 2:
                    generator.decreaseStep();
                    break;
            }

            return SignalPageResult::Redraw;


        case ButtonEvent::Confirm:

            editing = false;

            resetHold();

            return SignalPageResult::Redraw;


        case ButtonEvent::Back:

            editing = false;

            resetHold();

            return SignalPageResult::Redraw;


        case ButtonEvent::None:

            return SignalPageResult::None;
    }


    return SignalPageResult::None;
}


// --------------------------------------------------
// Getters
// --------------------------------------------------

uint8_t SignalGeneratorPage::getSelectedIndex() const
{
    return selectedIndex;
}


bool SignalGeneratorPage::isEditing() const
{
    return editing;
}

void SignalGeneratorPage::applyAdjustment(
    ButtonEvent direction,
    SignalGenerator& generator
)
{
    bool increase =
        direction == ButtonEvent::Up;


    switch (selectedIndex)
    {
        // Frequency
        case 0:

            if (increase)
            {
                generator.increaseFrequency();
            }
            else
            {
                generator.decreaseFrequency();
            }

            break;


        // Duty
        case 1:

            if (increase)
            {
                generator.increaseDuty();
            }
            else
            {
                generator.decreaseDuty();
            }

            break;


        // Step
        case 2:

            if (increase)
            {
                generator.increaseStep();
            }
            else
            {
                generator.decreaseStep();
            }

            break;


        default:
            break;
    }
}

void SignalGeneratorPage::resetHold()
{
    activeHoldButton =
        ButtonEvent::None;

    holdStartTime = 0;

    lastRepeatTime = 0;
}

SignalPageResult SignalGeneratorPage::updateHold(
    const ButtonManager& buttons,
    SignalGenerator& generator
)
{
    // Long-press repeat is only active in edit mode.
    if (!editing)
    {
        resetHold();

        return SignalPageResult::None;
    }


    ButtonEvent heldButton =
        ButtonEvent::None;


    if (buttons.isHeld(ButtonEvent::Up))
    {
        heldButton =
            ButtonEvent::Up;
    }
    else if (buttons.isHeld(ButtonEvent::Down))
    {
        heldButton =
            ButtonEvent::Down;
    }


    // Neither adjustment button is held.
    if (heldButton == ButtonEvent::None)
    {
        resetHold();

        return SignalPageResult::None;
    }


    unsigned long now =
        millis();


    // A new hold has just started.
    if (activeHoldButton != heldButton)
    {
        activeHoldButton =
            heldButton;

        holdStartTime =
            now;

        lastRepeatTime =
            now;

        return SignalPageResult::None;
    }


    // Wait before starting auto-repeat.
    if (
        (now - holdStartTime) <
        HOLD_DELAY_MS
    )
    {
        return SignalPageResult::None;
    }


    // Repeat at a controlled rate.
    if (
        (now - lastRepeatTime) >=
        REPEAT_INTERVAL_MS
    )
    {
        lastRepeatTime =
            now;


        applyAdjustment(
            heldButton,
            generator
        );


        return SignalPageResult::Redraw;
    }


    return SignalPageResult::None;
}