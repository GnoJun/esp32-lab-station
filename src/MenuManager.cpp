#include "MenuManager.h"


MenuManager::MenuManager()
    : selectedIndex(0),
      toolsSelectedIndex(0),
      timerAlarmSelectedIndex(0),
      currentPage(AppPage::MainMenu)
{
}


// --------------------------------------------------
// Begin
// --------------------------------------------------

void MenuManager::begin()
{
    selectedIndex = 0;

    toolsSelectedIndex = 0;

    timerAlarmSelectedIndex = 0;

    currentPage = AppPage::MainMenu;
}


// --------------------------------------------------
// Handle Button Event
// --------------------------------------------------

bool MenuManager::handleEvent(ButtonEvent event)
{
    bool stateChanged = false;


    // ==================================================
    // MAIN MENU
    // ==================================================

    if (currentPage == AppPage::MainMenu)
    {
        switch (event)
        {
            case ButtonEvent::Up:

                if (selectedIndex == 0)
                {
                    selectedIndex =
                        MAIN_MENU_ITEM_COUNT - 1;
                }
                else
                {
                    selectedIndex--;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Down:

                selectedIndex++;

                if (selectedIndex >= MAIN_MENU_ITEM_COUNT)
                {
                    selectedIndex = 0;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Confirm:

                switch (selectedIndex)
                {
                    case 0:
                        currentPage =
                            AppPage::Oscilloscope;
                        break;

                    case 1:
                        currentPage =
                            AppPage::SignalGenerator;
                        break;

                    case 2:
                        currentPage =
                            AppPage::Tools;
                        break;

                    case 3:
                        currentPage =
                            AppPage::Settings;
                        break;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Back:
            case ButtonEvent::None:
                break;
        }
    }


    // ==================================================
    // TOOLS MENU
    // ==================================================

    else if (currentPage == AppPage::Tools)
    {
        switch (event)
        {
            case ButtonEvent::Up:

                if (toolsSelectedIndex == 0)
                {
                    toolsSelectedIndex =
                        TOOLS_MENU_ITEM_COUNT - 1;
                }
                else
                {
                    toolsSelectedIndex--;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Down:

                toolsSelectedIndex++;

                if (toolsSelectedIndex >=
                    TOOLS_MENU_ITEM_COUNT)
                {
                    toolsSelectedIndex = 0;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Confirm:

                switch (toolsSelectedIndex)
                {
                    case 0:
                        currentPage =
                            AppPage::TimerAlarmMenu;
                        break;

                    case 1:
                        currentPage =
                            AppPage::I2CScanner;
                        break;

                    case 2:
                        currentPage =
                            AppPage::Voltmeter;
                        break;

                    case 3:
                        currentPage =
                            AppPage::FrequencyMeter;
                        break;

                    case 4:
                        currentPage =
                            AppPage::DigitalInput;
                        break;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Back:

                currentPage =
                    AppPage::MainMenu;

                stateChanged = true;
                break;


            case ButtonEvent::None:
                break;
        }
    }


    // ==================================================
    // TIMER / ALARM MENU
    // ==================================================

    else if (currentPage == AppPage::TimerAlarmMenu)
    {
        switch (event)
        {
            case ButtonEvent::Up:

                if (timerAlarmSelectedIndex == 0)
                {
                    timerAlarmSelectedIndex =
                        TIMER_ALARM_ITEM_COUNT - 1;
                }
                else
                {
                    timerAlarmSelectedIndex--;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Down:

                timerAlarmSelectedIndex++;

                if (timerAlarmSelectedIndex >=
                    TIMER_ALARM_ITEM_COUNT)
                {
                    timerAlarmSelectedIndex = 0;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Confirm:

                if (timerAlarmSelectedIndex == 0)
                {
                    currentPage =
                        AppPage::Timer;
                }
                else
                {
                    currentPage =
                        AppPage::Alarm;
                }

                stateChanged = true;
                break;


            case ButtonEvent::Back:

                currentPage =
                    AppPage::Tools;

                stateChanged = true;
                break;


            case ButtonEvent::None:
                break;
        }
    }


    // ==================================================
    // MAIN-LEVEL FUNCTION PAGES
    // ==================================================

    else if (
        currentPage == AppPage::Oscilloscope ||
        currentPage == AppPage::SignalGenerator ||
        currentPage == AppPage::Settings
    )
    {
        if (event == ButtonEvent::Back)
        {
            currentPage =
                AppPage::MainMenu;

            stateChanged = true;
        }
    }


    // ==================================================
    // TOOLS FUNCTION PAGES
    // ==================================================

    else if (
        currentPage == AppPage::I2CScanner ||
        currentPage == AppPage::Voltmeter ||
        currentPage == AppPage::FrequencyMeter ||
        currentPage == AppPage::DigitalInput
    )
    {
        if (event == ButtonEvent::Back)
        {
            currentPage =
                AppPage::Tools;

            stateChanged = true;
        }
    }


    // ==================================================
    // TIMER / ALARM FUNCTION PAGES
    // ==================================================

    else if (
        currentPage == AppPage::Timer ||
        currentPage == AppPage::Alarm
    )
    {
        if (event == ButtonEvent::Back)
        {
            currentPage =
                AppPage::TimerAlarmMenu;

            stateChanged = true;
        }
    }


    return stateChanged;
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

uint8_t MenuManager::getSelectedIndex() const
{
    return selectedIndex;
}

uint8_t MenuManager::getToolsSelectedIndex() const
{
    return toolsSelectedIndex;
}


uint8_t MenuManager::getTimerAlarmSelectedIndex() const
{
    return timerAlarmSelectedIndex;
}


AppPage MenuManager::getCurrentPage() const
{
    return currentPage;
}