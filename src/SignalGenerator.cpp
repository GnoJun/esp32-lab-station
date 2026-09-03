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
        return 15;
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

uint32_t SignalGenerator::getFrequencyStep() const
{
    if (frequencyHz < 20)
    {
        return 1;
    }

    if (frequencyHz < 100)
    {
        return 5;
    }

    if (frequencyHz < 1000)
    {
        return 50;
    }

    if (frequencyHz < 10000)
    {
        return 500;
    }

    return 1000;
}

bool SignalGenerator::increaseFrequency()
{
    uint32_t step =
        getFrequencyStep();


    if (frequencyHz >= MAX_FREQUENCY_HZ)
    {
        return false;
    }


    uint32_t newFrequency =
        frequencyHz + step;


    if (newFrequency > MAX_FREQUENCY_HZ)
    {
        newFrequency =
            MAX_FREQUENCY_HZ;
    }


    frequencyHz =
        newFrequency;


    return applySettings();
}

bool SignalGenerator::decreaseFrequency()
{
    uint32_t step =
        getFrequencyStep();


    if (frequencyHz <= MIN_FREQUENCY_HZ)
    {
        return false;
    }


    if (frequencyHz <= step)
    {
        frequencyHz =
            MIN_FREQUENCY_HZ;
    }
    else
    {
        frequencyHz -= step;
    }


    if (frequencyHz < MIN_FREQUENCY_HZ)
    {
        frequencyHz =
            MIN_FREQUENCY_HZ;
    }


    return applySettings();
}

bool SignalGenerator::toggleOutput()
{
    outputEnabled =
        !outputEnabled;


    return applySettings();
}

uint32_t SignalGenerator::getFrequency() const
{
    return frequencyHz;
}


uint8_t SignalGenerator::getDutyPercent() const
{
    return dutyPercent;
}


bool SignalGenerator::isOutputEnabled() const
{
    return outputEnabled;
}

