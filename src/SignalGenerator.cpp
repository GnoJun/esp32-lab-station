#include "SignalGenerator.h"

#include "pins.h"

#include "driver/ledc.h"

bool SignalGenerator::begin()
{
    // ----------------------------------------------
    // Configure LEDC timer
    // ----------------------------------------------

    resolutionBits =
        chooseResolution(frequencyHz);


    ledc_timer_config_t timerConfig = {};

    timerConfig.speed_mode =
        LEDC_LOW_SPEED_MODE;

    timerConfig.timer_num =
        LEDC_TIMER_0;

    timerConfig.duty_resolution =
        static_cast<ledc_timer_bit_t>(
            resolutionBits
        );

    timerConfig.freq_hz =
        frequencyHz;

    timerConfig.clk_cfg =
        LEDC_AUTO_CLK;


    if (ledc_timer_config(&timerConfig) != ESP_OK)
    {
        return false;
    }


    // ----------------------------------------------
    // Configure GPIO4 output channel
    // ----------------------------------------------

    ledc_channel_config_t channelConfig = {};

    channelConfig.gpio_num =
        Pins::SIGNAL_OUT;

    channelConfig.speed_mode =
        LEDC_LOW_SPEED_MODE;

    channelConfig.channel =
        LEDC_CHANNEL_0;

    channelConfig.intr_type =
        LEDC_INTR_DISABLE;

    channelConfig.timer_sel =
        LEDC_TIMER_0;

    channelConfig.duty = 0;

    channelConfig.hpoint = 0;


    if (ledc_channel_config(&channelConfig) != ESP_OK)
    {
        return false;
    }


    outputEnabled = false;

    return true;
}

uint8_t SignalGenerator::chooseResolution(
    uint32_t frequency
) const
{
    if (frequency <= 20)
    {
        return 14;
    }

    if (frequency <= 200)
    {
        return 14;
    }

    if (frequency <= 2000)
    {
        return 13;
    }

    if (frequency <= 10000)
    {
        return 11;
    }

    return 10;
}

bool SignalGenerator::applySettings()
{
    resolutionBits =
        chooseResolution(frequencyHz);


    // ----------------------------------------------
    // Reconfigure timer
    // ----------------------------------------------

    ledc_timer_config_t timerConfig = {};

    timerConfig.speed_mode =
        LEDC_LOW_SPEED_MODE;

    timerConfig.timer_num =
        LEDC_TIMER_0;

    timerConfig.duty_resolution =
        static_cast<ledc_timer_bit_t>(
            resolutionBits
        );

    timerConfig.freq_hz =
        frequencyHz;

    timerConfig.clk_cfg =
        LEDC_AUTO_CLK;


    if (ledc_timer_config(&timerConfig) != ESP_OK)
    {
        return false;
    }


    // ----------------------------------------------
    // Calculate duty
    // ----------------------------------------------

    uint32_t maxDuty =
        (1UL << resolutionBits) - 1;

    uint32_t duty = 0;


    if (outputEnabled)
    {
        duty =
            (maxDuty * dutyPercent) / 100;
    }


    // ----------------------------------------------
    // Apply duty
    // ----------------------------------------------

    if (
        ledc_set_duty(
            LEDC_LOW_SPEED_MODE,
            LEDC_CHANNEL_0,
            duty
        ) != ESP_OK
    )
    {
        return false;
    }


    if (
        ledc_update_duty(
            LEDC_LOW_SPEED_MODE,
            LEDC_CHANNEL_0
        ) != ESP_OK
    )
    {
        return false;
    }


    return true;
}


bool SignalGenerator::increaseFrequency()
{
    if (frequencyHz >= MAX_FREQUENCY_HZ)
    {
        return false;
    }


    uint32_t oldFrequency =
        frequencyHz;


    if (
        stepHz >
        (MAX_FREQUENCY_HZ - frequencyHz)
    )
    {
        frequencyHz =
            MAX_FREQUENCY_HZ;
    }
    else
    {
        frequencyHz += stepHz;
    }


    if (!applySettings())
    {
        frequencyHz =
            oldFrequency;

        applySettings();

        return false;
    }


    return true;
}

bool SignalGenerator::decreaseFrequency()
{
    if (frequencyHz <= MIN_FREQUENCY_HZ)
    {
        return false;
    }


    uint32_t oldFrequency =
        frequencyHz;


    if (
        frequencyHz <=
        MIN_FREQUENCY_HZ + stepHz
    )
    {
        frequencyHz =
            MIN_FREQUENCY_HZ;
    }
    else
    {
        frequencyHz -= stepHz;
    }


    if (!applySettings())
    {
        frequencyHz =
            oldFrequency;

        applySettings();

        return false;
    }


    return true;
}

bool SignalGenerator::toggleOutput()
{
    bool oldState =
        outputEnabled;


    outputEnabled =
        !outputEnabled;


    if (!applySettings())
    {
        outputEnabled =
            oldState;

        applySettings();

        return false;
    }


    return true;
}

uint32_t SignalGenerator::getFrequency() const
{
    return frequencyHz;
}


uint8_t SignalGenerator::getDutyPercent() const
{
    return dutyPercent;
}

uint32_t SignalGenerator::getStepHz() const
{
    return stepHz;
}

bool SignalGenerator::isOutputEnabled() const
{
    return outputEnabled;
}

bool SignalGenerator::increaseDuty()
{
    if (dutyPercent >= 99)
    {
        return false;
    }


    uint8_t oldDuty =
        dutyPercent;


    dutyPercent++;


    if (!applySettings())
    {
        dutyPercent =
            oldDuty;

        applySettings();

        return false;
    }


    return true;
}

bool SignalGenerator::decreaseDuty()
{
    if (dutyPercent <= 1)
    {
        return false;
    }


    uint8_t oldDuty =
        dutyPercent;


    dutyPercent--;


    if (!applySettings())
    {
        dutyPercent =
            oldDuty;

        applySettings();

        return false;
    }


    return true;
}

bool SignalGenerator::increaseStep()
{
    switch (stepHz)
    {
        case 1:
            stepHz = 10;
            break;

        case 10:
            stepHz = 100;
            break;

        case 100:
            stepHz = 1000;
            break;

        case 1000:
            return false;

        default:
            stepHz = 100;
            break;
    }


    return true;
}

bool SignalGenerator::decreaseStep()
{
    switch (stepHz)
    {
        case 1000:
            stepHz = 100;
            break;

        case 100:
            stepHz = 10;
            break;

        case 10:
            stepHz = 1;
            break;

        case 1:
            return false;

        default:
            stepHz = 100;
            break;
    }


    return true;
}