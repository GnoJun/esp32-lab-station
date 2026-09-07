#include "DisplayManager.h"


DisplayManager::DisplayManager(
    U8G2& mainDisplay,
    U8G2& menuDisplay
)
    : mainDisplay(mainDisplay),
      menuDisplay(menuDisplay)
{
}


// --------------------------------------------------
// Begin
// --------------------------------------------------

void DisplayManager::begin()
{
    mainDisplay.begin();
    menuDisplay.begin();

    mainDisplay.setFont(u8g2_font_6x12_tf);
    menuDisplay.setFont(u8g2_font_6x12_tf);
}


// --------------------------------------------------
// Boot Screen
// --------------------------------------------------

void DisplayManager::showBootScreen()
{
    // Main OLED
    mainDisplay.clearBuffer();

    mainDisplay.setFont(u8g2_font_6x12_tf);

    mainDisplay.drawStr(
        14,
        26,
        "ESP32 LAB STATION"
    );

    mainDisplay.drawStr(
        31,
        44,
        "Starting..."
    );

    mainDisplay.sendBuffer();


    // Menu OLED
    menuDisplay.clearBuffer();

    menuDisplay.setFont(u8g2_font_6x12_tf);

    menuDisplay.drawStr(
        26,
        26,
        "SYSTEM"
    );

    menuDisplay.drawStr(
        35,
        44,
        "BOOT"
    );

    menuDisplay.sendBuffer();
}


void DisplayManager::showMainMenu(
    uint8_t selectedIndex
)
{
    const char* menuItems[] =
    {
        "Oscilloscope",
        "Signal Gen",
        "Tools",
        "Settings"
    };


    // ----------------------------------------------
    // Main OLED
    // ----------------------------------------------

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );

    mainDisplay.drawStr(
        0,
        11,
        "ESP32 LAB STATION"
    );

    mainDisplay.drawHLine(
        0,
        15,
        128
    );

    mainDisplay.drawStr(
        0,
        33,
        "Selected:"
    );

    mainDisplay.drawStr(
        0,
        50,
        menuItems[selectedIndex]
    );

    mainDisplay.sendBuffer();


    // ----------------------------------------------
    // Menu OLED
    // ----------------------------------------------

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );

    menuDisplay.drawStr(
        0,
        10,
        "MAIN MENU"
    );

    menuDisplay.drawHLine(
        0,
        13,
        128
    );


    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t y =
            25 + i * 12;

        if (i == selectedIndex)
        {
            menuDisplay.drawStr(
                0,
                y,
                ">"
            );
        }

        menuDisplay.drawStr(
            10,
            y,
            menuItems[i]
        );
    }


    menuDisplay.sendBuffer();
}

void DisplayManager::showToolPage(
    const char* title
)
{
    // ----------------------------------------------
    // Main OLED
    // ----------------------------------------------

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );

    mainDisplay.drawStr(
        0,
        11,
        title
    );

    mainDisplay.drawHLine(
        0,
        15,
        128
    );

    mainDisplay.drawStr(
        0,
        34,
        "Module selected"
    );

    mainDisplay.drawStr(
        0,
        50,
        "Coming soon..."
    );

    mainDisplay.sendBuffer();


    // ----------------------------------------------
    // Menu OLED
    // ----------------------------------------------

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );

    menuDisplay.drawStr(
        0,
        11,
        title
    );

    menuDisplay.drawHLine(
        0,
        15,
        128
    );

    menuDisplay.drawStr(
        0,
        35,
        "BACK: Return"
    );

    menuDisplay.sendBuffer();
}

void DisplayManager::showSubMenu(
    const char* title,
    const char* const items[],
    uint8_t itemCount,
    uint8_t selectedIndex
)
{
    // OLED can comfortably display four menu items
    constexpr uint8_t VISIBLE_ITEMS = 4;


    // Determine which menu item should appear first
    uint8_t firstVisibleIndex = 0;

    if (selectedIndex >= VISIBLE_ITEMS)
    {
        firstVisibleIndex =
            selectedIndex - VISIBLE_ITEMS + 1;
    }


    // --------------------------------------------------
    // Main OLED
    // --------------------------------------------------

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );

    mainDisplay.drawStr(
        0,
        11,
        title
    );

    mainDisplay.drawHLine(
        0,
        15,
        128
    );

    mainDisplay.drawStr(
        0,
        33,
        "Selected:"
    );

    mainDisplay.drawStr(
        0,
        50,
        items[selectedIndex]
    );

    mainDisplay.sendBuffer();


    // --------------------------------------------------
    // Menu OLED
    // --------------------------------------------------

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );

    menuDisplay.drawStr(
        0,
        10,
        title
    );

    menuDisplay.drawHLine(
        0,
        13,
        128
    );


    for (uint8_t row = 0;
         row < VISIBLE_ITEMS;
         row++)
    {
        uint8_t itemIndex =
            firstVisibleIndex + row;


        if (itemIndex >= itemCount)
        {
            break;
        }


        uint8_t y =
            25 + row * 12;


        if (itemIndex == selectedIndex)
        {
            menuDisplay.drawStr(
                0,
                y,
                ">"
            );
        }


        menuDisplay.drawStr(
            10,
            y,
            items[itemIndex]
        );
    }


    menuDisplay.sendBuffer();
}

void DisplayManager::showI2CScanner(
    const uint8_t addresses[],
    uint8_t deviceCount
)
{
    // --------------------------------------------------
    // Main OLED
    // --------------------------------------------------

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );

    mainDisplay.drawStr(
        0,
        11,
        "I2C SCANNER"
    );

    mainDisplay.drawHLine(
        0,
        15,
        128
    );


    char countText[24];

    snprintf(
        countText,
        sizeof(countText),
        "Devices found: %u",
        deviceCount
    );

    mainDisplay.drawStr(
        0,
        34,
        countText
    );

    mainDisplay.drawStr(
        0,
        52,
        "CONFIRM: Rescan"
    );

    mainDisplay.sendBuffer();


    // --------------------------------------------------
    // Menu OLED
    // --------------------------------------------------

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );

    menuDisplay.drawStr(
        0,
        10,
        "I2C DEVICES"
    );

    menuDisplay.drawHLine(
        0,
        13,
        128
    );


    if (deviceCount == 0)
    {
        menuDisplay.drawStr(
            0,
            32,
            "No devices found"
        );
    }
    else
    {
        uint8_t visibleCount =
            deviceCount;

        if (visibleCount > 4)
        {
            visibleCount = 4;
        }


        for (uint8_t i = 0;
             i < visibleCount;
             i++)
        {
            char addressText[16];

            snprintf(
                addressText,
                sizeof(addressText),
                "%u: 0x%02X",
                i + 1,
                addresses[i]
            );


            uint8_t y =
                25 + i * 12;

            menuDisplay.drawStr(
                0,
                y,
                addressText
            );
        }
    }


    menuDisplay.sendBuffer();
}


void DisplayManager::showSignalGenerator(
    uint32_t frequencyHz,
    uint8_t dutyPercent,
    uint32_t stepHz,
    bool outputEnabled,
    uint8_t selectedIndex,
    bool editing
)
{
    // ==================================================
    // Main OLED
    // ==================================================

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );


    mainDisplay.drawStr(
        0,
        10,
        "SIGNAL GENERATOR"
    );

    mainDisplay.drawHLine(
        0,
        13,
        128
    );


    char frequencyText[22];
    char dutyText[22];
    char stepText[22];
    char outputText[22];


    snprintf(
        frequencyText,
        sizeof(frequencyText),
        "Freq   %lu Hz",
        static_cast<unsigned long>(
            frequencyHz
        )
    );


    snprintf(
        dutyText,
        sizeof(dutyText),
        "Duty   %u%%",
        dutyPercent
    );


    snprintf(
        stepText,
        sizeof(stepText),
        "Step   %lu Hz",
        static_cast<unsigned long>(
            stepHz
        )
    );


    snprintf(
        outputText,
        sizeof(outputText),
        "Output %s",
        outputEnabled ? "ON" : "OFF"
    );


    const char* rows[] =
    {
        frequencyText,
        dutyText,
        stepText,
        outputText
    };


    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t y =
            25 + i * 12;


        if (i == selectedIndex)
        {
            if (editing)
            {
                mainDisplay.drawStr(
                    0,
                    y,
                    "*"
                );
            }
            else
            {
                mainDisplay.drawStr(
                    0,
                    y,
                    ">"
                );
            }
        }


        mainDisplay.drawStr(
            10,
            y,
            rows[i]
        );
    }


    mainDisplay.sendBuffer();


    // ==================================================
    // Menu OLED
    // ==================================================

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );


    if (editing)
    {
        menuDisplay.drawStr(
            0,
            10,
            "EDIT MODE"
        );

        menuDisplay.drawHLine(
            0,
            13,
            128
        );

        menuDisplay.drawStr(
            0,
            28,
            "UP:      Increase"
        );

        menuDisplay.drawStr(
            0,
            40,
            "DOWN:    Decrease"
        );

        menuDisplay.drawStr(
            0,
            52,
            "OK:        Done"
        );

        menuDisplay.drawStr(
            0,
            64,
            "BACK:      Done"
        );
    }
    else
    {
        menuDisplay.drawStr(
            0,
            10,
            "SIGNAL GEN"
        );

        menuDisplay.drawHLine(
            0,
            13,
            128
        );

        menuDisplay.drawStr(
            0,
            28,
            "UP/DOWN:      Select"
        );

        menuDisplay.drawStr(
            0,
            40,
            "OK:          Confirm"
        );

        menuDisplay.drawStr(
            0,
            52,
            "BACK:      Main Menu"
        );
    }


    menuDisplay.sendBuffer();
}

void DisplayManager::showOscilloscopeBaseline(
    uint16_t averageRaw,
    uint32_t millivolts,
    uint16_t minRaw,
    uint16_t maxRaw
)
{
    // ==================================================
    // Main OLED
    // ==================================================

    mainDisplay.clearBuffer();

    mainDisplay.setFont(
        u8g2_font_6x12_tf
    );


    mainDisplay.drawStr(
        0,
        10,
        "SCOPE ADC TEST"
    );

    mainDisplay.drawHLine(
        0,
        13,
        128
    );


    char rawText[24];

    snprintf(
        rawText,
        sizeof(rawText),
        "Raw avg: %u",
        averageRaw
    );


    mainDisplay.drawStr(
        0,
        28,
        rawText
    );


    char voltageText[24];

    snprintf(
        voltageText,
        sizeof(voltageText),
        "ADC pin: %lu mV",
        static_cast<unsigned long>(
            millivolts
        )
    );


    mainDisplay.drawStr(
        0,
        42,
        voltageText
    );


    char rangeText[24];

    snprintf(
        rangeText,
        sizeof(rangeText),
        "Min:%u Max:%u",
        minRaw,
        maxRaw
    );


    mainDisplay.drawStr(
        0,
        57,
        rangeText
    );


    mainDisplay.sendBuffer();


    // ==================================================
    // Menu OLED
    // ==================================================

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );


    menuDisplay.drawStr(
        0,
        10,
        "OSCILLOSCOPE"
    );

    menuDisplay.drawHLine(
        0,
        13,
        128
    );


    menuDisplay.drawStr(
        0,
        28,
        "GPIO1 / ADC1"
    );


    menuDisplay.drawStr(
        0,
        40,
        "12-bit / LIVE"
    );


    menuDisplay.drawStr(
        0,
        52,
        "Value = ADC pin"
    );


    menuDisplay.drawStr(
        0,
        64,
        "BACK: Return"
    );


    menuDisplay.sendBuffer();
}