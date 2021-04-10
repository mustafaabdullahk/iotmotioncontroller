#include "src/driver.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS);

Config::Config()
{
    spi.format(8);
    spi.frequency(1000000);
}

int Config::WriteByte(int value)
{
    cs = false;
    auto state = spi.write(value);
    cs = true;
    if(state)
        return state;
    return 0;
}
char* Config::WriteBytes(const char *txData, int txSize, char *rxData, int rxSize)
{
    cs = false;
    spi.write(txData, txSize, rxData, rxSize);
    cs = true;
    return rxData;
}