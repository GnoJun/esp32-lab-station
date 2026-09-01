#include "I2CScanner.h"


// --------------------------------------------------
// Begin
// --------------------------------------------------

void I2CScanner::begin()
{
    deviceCount = 0;
}


// --------------------------------------------------
// Scan
// --------------------------------------------------

void I2CScanner::scan()
{
    deviceCount = 0;


    for (uint8_t address = 1;
         address < 127;
         address++)
    {
        Wire.beginTransmission(address);

        uint8_t error =
            Wire.endTransmission();


        if (error == 0)
        {
            if (deviceCount < MAX_DEVICES)
            {
                addresses[deviceCount] =
                    address;

                deviceCount++;
            }
        }
    }
}


// --------------------------------------------------
// Get Device Count
// --------------------------------------------------

uint8_t I2CScanner::getDeviceCount() const
{
    return deviceCount;
}


// --------------------------------------------------
// Get Address
// --------------------------------------------------

uint8_t I2CScanner::getAddress(
    uint8_t index
) const
{
    if (index >= deviceCount)
    {
        return 0;
    }

    return addresses[index];
}