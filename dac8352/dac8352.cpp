#include "dac8352.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <string>


int Digital2Analog::WriteData(uint8_t Channel, uint16_t Data)
{
    uint8_t bitShift = Data >> 8;
    uint8_t bitMask = Data & 0xFF;
    std::string str;
    str.push_back(Channel);
    str.push_back(bitShift);
    str.push_back(bitMask);
    char rxData;
    spiHandle.WriteBytes(str.c_str(), str.size(), &rxData, sizeof(rxData));
    printf("DAC Write Byte Value: %u ", str.size());
    return 0;
}

int Digital2Analog::OutVoltage(uint8_t Channel, uint16_t Voltage)
{
    if ((Voltage <= DAC_VREF) && (Voltage >= 0)) 
    {
        uint16_t temp = (Voltage * (DAC_Value_MAX / DAC_VREF));
        WriteData(Channel, temp);
    }
    return 0;
}
