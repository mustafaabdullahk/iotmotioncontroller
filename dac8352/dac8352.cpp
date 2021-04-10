#include "dac8352.h"
#include <cstdint>

int Digital2Analog::WriteData(char Channel, char Data)
{
    DigitalOut chipSelect(SPI_CS);
    uint8_t bitShift = Data >> 8;
    uint8_t bitMask = Data & 0xFF;
    char *txData;
    txData[0] = Channel;
    txData[1] = (Data >> 8);
    txData[2] = (Data & 0xFF);
    char rxData;
    chipSelect = false;
    spiHandle.WriteBytes(txData, sizeof(txData), &rxData, sizeof(rxData));
    chipSelect = true;
    return 0;
}

int Digital2Analog::OutVoltage(char Channel, float Voltage)
{
    if ((Voltage <= DAC_VREF) && (Voltage >= 0)) 
    {
        float temp = (Voltage * (DAC_Value_MAX / DAC_VREF));
        WriteData(Channel, temp);
    }
    return 0;
}
