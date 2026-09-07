#include <Arduino.h>
#include <Wire.h>

#include "pins.h"
#include "ButtonManager.h"
#include "DisplayManager.h"
#include "MenuManager.h"
#include "I2CScanner.h"
#include "SignalGenerator.h"
#include "SignalGeneratorPage.h"
#include "OscilloscopeInput.h"

ButtonManager buttonManager;
MenuManager menuManager;
I2CScanner i2cScanner;
SignalGenerator signalGenerator;
SignalGeneratorPage signalGeneratorPage;
OscilloscopeInput oscilloscopeInput;

// Main OLED: I2C 0x3C -> U8g2 address 0x78
U8G2_SSD1306_128X64_NONAME_F_HW_I2C
    u8g2_main(U8G2_R0, U8X8_PIN_NONE);


// Menu OLED: I2C 0x3D -> U8g2 address 0x7A
U8G2_SSD1306_128X64_NONAME_F_HW_I2C
    u8g2_menu(U8G2_R0, U8X8_PIN_NONE);


DisplayManager displayManager(
    u8g2_main,
    u8g2_menu
);

// --------------------------------------------------
// Buzzer
// --------------------------------------------------

void beep(uint16_t frequency, uint16_t durationMs)
{
    if (frequency == 0)
        return;

    uint32_t halfPeriodUs = 500000UL / frequency;

    uint32_t cycles =
        static_cast<uint32_t>(frequency) * durationMs / 1000UL;

    for (uint32_t i = 0; i < cycles; i++)
    {
        digitalWrite(Pins::BUZZER, HIGH);
        delayMicroseconds(halfPeriodUs);

        digitalWrite(Pins::BUZZER, LOW);
        delayMicroseconds(halfPeriodUs);
    }
}


// --------------------------------------------------
// I2C Scanner
// --------------------------------------------------

void scanI2C()
{
    Serial.println();
    Serial.println("Scanning I2C bus...");

    uint8_t deviceCount = 0;

    for (uint8_t address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);

        uint8_t error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("Found I2C device at 0x");

            if (address < 0x10)
                Serial.print("0");

            Serial.println(address, HEX);

            deviceCount++;
        }
    }

    Serial.print("I2C scan complete. Devices found: ");
    Serial.println(deviceCount);

    Serial.println();
}




// --------------------------------------------------
// Setup
// --------------------------------------------------

const char* const TOOLS_MENU_ITEMS[] =
{
    "Timer / Alarm",
    "I2C Scanner",
    "Voltmeter",
    "Frequency Meter",
    "Digital Input"
};


const char* const TIMER_ALARM_ITEMS[] =
{
    "Timer",
    "Alarm"
};

void updateUI()
{
    AppPage page =
        menuManager.getCurrentPage();


    switch (page)
    {
        // ==========================================
        // Main Menu
        // ==========================================

        case AppPage::MainMenu:

            displayManager.showMainMenu(
                menuManager.getSelectedIndex()
            );

            break;


        // ==========================================
        // Main function pages
        // ==========================================

        case AppPage::Oscilloscope:

            displayManager.showOscilloscopeBaseline(
                oscilloscopeInput.getAverageRaw(),
                oscilloscopeInput.getAverageMillivolts(),
                oscilloscopeInput.getInputMillivolts(),
                oscilloscopeInput.getMinRaw(),
                oscilloscopeInput.getMaxRaw()
            );

            break;


        case AppPage::SignalGenerator:

            displayManager.showSignalGenerator(
                signalGenerator.getFrequency(),
                signalGenerator.getDutyPercent(),
                signalGenerator.getStepHz(),
                signalGenerator.isOutputEnabled(),
                signalGeneratorPage.getSelectedIndex(),
                signalGeneratorPage.isEditing()
            );

            break;


        // ==========================================
        // Tools Menu
        // ==========================================

        case AppPage::Tools:

            displayManager.showSubMenu(
                "TOOLS",
                TOOLS_MENU_ITEMS,
                5,
                menuManager.getToolsSelectedIndex()
            );

            break;


        // ==========================================
        // Timer / Alarm Menu
        // ==========================================

        case AppPage::TimerAlarmMenu:

            displayManager.showSubMenu(
                "TIMER / ALARM",
                TIMER_ALARM_ITEMS,
                2,
                menuManager.getTimerAlarmSelectedIndex()
            );

            break;


        // ==========================================
        // Tool pages
        // ==========================================

        case AppPage::I2CScanner:
        {
            uint8_t addresses[16];

            uint8_t count =
                i2cScanner.getDeviceCount();


            for (uint8_t i = 0; i < count; i++)
            {
                addresses[i] =
                    i2cScanner.getAddress(i);
            }


            displayManager.showI2CScanner(
                addresses,
                count
            );

            break;
        }


        case AppPage::Voltmeter:

            displayManager.showToolPage(
                "VOLTMETER"
            );

            break;


        case AppPage::FrequencyMeter:

            displayManager.showToolPage(
                "FREQUENCY METER"
            );

            break;


        case AppPage::DigitalInput:

            displayManager.showToolPage(
                "DIGITAL INPUT"
            );

            break;


        // ==========================================
        // Timer / Alarm pages
        // ==========================================

        case AppPage::Timer:

            displayManager.showToolPage(
                "TIMER"
            );

            break;


        case AppPage::Alarm:

            displayManager.showToolPage(
                "ALARM"
            );

            break;


        // ==========================================
        // Settings
        // ==========================================

        case AppPage::Settings:

            displayManager.showToolPage(
                "SETTINGS"
            );

            break;
    }
}

void setup()
{
    Serial.begin(115200);
    buttonManager.begin();
    delay(500);

    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 Lab Station");
    Serial.println("Hardware Baseline Test");
    Serial.println("==============================");





    // -------------------------
    // Buzzer
    // -------------------------

    pinMode(Pins::BUZZER, OUTPUT);

    digitalWrite(Pins::BUZZER, LOW);



    


    // -------------------------
    // I2C
    // -------------------------

    Wire.begin(
        Pins::I2C_SDA,
        Pins::I2C_SCL
    );
  

    // -------------------------
    // OLED addresses
    // -------------------------

    u8g2_main.setI2CAddress(0x78);
    u8g2_menu.setI2CAddress(0x7A);

    i2cScanner.begin();

    oscilloscopeInput.begin();

    if (!signalGenerator.begin())
    {
        Serial.println(
            "ERROR: Signal Generator init failed"
        );
    }
    else
    {
        Serial.println(
            "Signal Generator ready"
        );
    }

    signalGeneratorPage.begin();

    displayManager.begin();
    menuManager.begin();

    displayManager.showBootScreen();

    delay(1000);

    updateUI();

    // -------------------------
    // Hardware tests
    // -------------------------

    scanI2C();

    Serial.println("Testing buzzer...");

    //beep(2000, 120);//

    Serial.println("Buzzer test skipped (muted).");
    Serial.println();

    Serial.println("Press buttons to test them.");
    Serial.println();
}


// --------------------------------------------------
// Main Loop
// --------------------------------------------------

void loop()
{
    ButtonEvent event =
        buttonManager.update();


    AppPage pageBefore =
        menuManager.getCurrentPage();

    // --------------------------------------------------
    // Oscilloscope ADC baseline
    // --------------------------------------------------

    if (pageBefore == AppPage::Oscilloscope)
    {
        static unsigned long lastScopeUpdate = 0;

        unsigned long now =
            millis();


        // Refresh about 5 times per second.
        if (
            now - lastScopeUpdate >= 200
        )
        {
            lastScopeUpdate =
                now;

            oscilloscopeInput.sample();

            updateUI();
        }


        // BACK still belongs to MenuManager.
        if (event == ButtonEvent::Back)
        {
            bool changed =
                menuManager.handleEvent(
                    ButtonEvent::Back
                );


            if (changed)
            {
                updateUI();
            }
        }


        return;
    }


    // --------------------------------------------------
    // Signal Generator
    // --------------------------------------------------

    if (pageBefore == AppPage::SignalGenerator)
    {
        // Handle normal press events
        if (event != ButtonEvent::None)
        {
            SignalPageResult result =
                signalGeneratorPage.handleEvent(
                    event,
                    signalGenerator
                );


            if (
                result ==
                SignalPageResult::Redraw
            )
            {
                updateUI();

                return;
            }


            if (
                result ==
                SignalPageResult::ExitRequested
            )
            {
                bool changed =
                    menuManager.handleEvent(
                        ButtonEvent::Back
                    );


                if (changed)
                {
                    updateUI();
                }


                return;
            }
        }


        // Handle long-press auto repeat
        SignalPageResult holdResult =
            signalGeneratorPage.updateHold(
                buttonManager,
                signalGenerator
            );


        if (
            holdResult ==
            SignalPageResult::Redraw
        )
        {
            updateUI();
        }


        return;
    }


    // Other pages still require a button event
    if (event == ButtonEvent::None)
    {
        return;
    }


    // Existing I2C Scanner / Menu code continues below...

    // --------------------------------------------------
    // I2C Scanner internal controls
    // --------------------------------------------------

    if (pageBefore == AppPage::I2CScanner)
    {
        if (event == ButtonEvent::Confirm)
        {
            i2cScanner.scan();

            updateUI();

            return;
        }
    }


    // --------------------------------------------------
    // Menu navigation
    // --------------------------------------------------

    bool changed =
        menuManager.handleEvent(event);


    if (!changed)
    {
        return;
    }


    AppPage pageAfter =
        menuManager.getCurrentPage();


    // Entering I2C Scanner
    if (
        pageBefore != AppPage::I2CScanner &&
        pageAfter == AppPage::I2CScanner
    )
    {
        i2cScanner.scan();
    }

    if (
        pageBefore != AppPage::Oscilloscope &&
        pageAfter == AppPage::Oscilloscope
    )
    {
        oscilloscopeInput.sample();
    }


    updateUI();
}