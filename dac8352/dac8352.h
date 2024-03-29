#ifndef MBED_DIGITAL2ANALOG_H
#define MBED_DIGITAL2ANALOG_H

#include <cstdint>
#include "src/driver.h"

class Digital2Analog{

public:
    //SPI spi(D11, D12, D13); // mosi, miso, sclk
  Digital2Analog() {}
  int WriteData(uint8_t Channel, uint16_t Data);
  int OutVoltage(uint8_t Channel, uint16_t Data);
public:
    char channel_A   = 0x30;
    char channel_B   = 0x34;
    int DAC_Value_MAX   = 65535;
    float DAC_VREF      = 3.3;
private:
    Config spiHandle;
};
#endif