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
    bool outputEnabled
)
{
    // ==============================================
    // Main OLED
    // ==============================================

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


    char frequencyText[24];

    snprintf(
        frequencyText,
        sizeof(frequencyText),
        "Freq: %lu Hz",
        static_cast<unsigned long>(
            frequencyHz
        )
    );


    mainDisplay.drawStr(
        0,
        28,
        frequencyText
    );


    char dutyText[20];

    snprintf(
        dutyText,
        sizeof(dutyText),
        "Duty: %u%%",
        dutyPercent
    );


    mainDisplay.drawStr(
        0,
        41,
        dutyText
    );


    if (outputEnabled)
    {
        mainDisplay.drawStr(
            0,
            56,
            "Output: ON"
        );
    }
    else
    {
        mainDisplay.drawStr(
            0,
            56,
            "Output: OFF"
        );
    }


    mainDisplay.sendBuffer();


    // ==============================================
    // Menu OLED
    // ==============================================

    menuDisplay.clearBuffer();

    menuDisplay.setFont(
        u8g2_font_6x12_tf
    );


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
        27,
        "UP:   Frequency +"
    );

    menuDisplay.drawStr(
        0,
        39,
        "DOWN: Frequency -"
    );

    menuDisplay.drawStr(
        0,
        51,
        "OK: Output ON/OFF"
    );

    menuDisplay.drawStr(
        0,
        63,
        "BACK: Return"
    );


    menuDisplay.sendBuffer();
}