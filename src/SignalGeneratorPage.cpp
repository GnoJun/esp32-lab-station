#include "SignalGeneratorPage.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void SignalGeneratorPage::begin()
{
    selectedIndex = 0;

    editing = false;
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

            return SignalPageResult::Redraw;


        case ButtonEvent::Back:

            editing = false;

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