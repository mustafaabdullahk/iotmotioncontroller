#ifndef DIGITAL2ANALOG
#define DIGITAL2ANALOG

#include "platform/platform.h"
#include "hal/gpio_api.h"
#include "hal/spi_api.h"

class Digital2Analog{
    
public:
    //SPI spi(D11, D12, D13); // mosi, miso, sclk
    Digital2Analog(PinName mosi, PinName miso, PinName sclk)
    {

    }
    int WriteData(uint8_t Channel, uint8_t Data)
    {

        return 0;
    }

};
#endif