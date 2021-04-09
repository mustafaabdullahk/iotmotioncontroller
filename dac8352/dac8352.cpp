#include "dac8352.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS);


Digital2Analog::Digital2Analog()
{
  spi.format(8);
  spi.frequency(1000000);
}

int Digital2Analog::WriteData(uint8_t Channel, uint8_t Data)
{
    return 0;
}