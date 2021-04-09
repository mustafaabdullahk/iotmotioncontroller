#ifndef MBED_DIGITAL2ANALOG_H
#define MBED_DIGITAL2ANALOG_H

#include "mbed.h"
#include "hal/spi_api.h"



class Digital2Analog{

public:
    //SPI spi(D11, D12, D13); // mosi, miso, sclk
    Digital2Analog();
    int WriteData(uint8_t Channel, uint8_t Data);
};
#endif