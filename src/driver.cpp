#include "src/driver.h"
#include <cstdio>
#include <cstring>
#include <string>

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS); //chip select

Config::Config()
{
    spi.format(8);
    spi.frequency(1000000);
}

int Config::WriteByte(int value)
{
    cs = 0;
    auto state = spi.write(value);
    cs = 1;
    if(state != 0) {
        return state;
    }
    return 0;
}
char* Config::WriteBytes(const char *txData, int txSize, char *rxData, int rxSize)
{
    cs = 0;
    auto test = spi.write(txData, txSize, rxData, rxSize);
    cs = 1;
    std::string str;
    memcpy(&str,rxData,sizeof(rxData));
    printf("stringggggggggg %s", rxData);
    printf("testtttttttt %s", sizeof(rxData));
    return rxData;
}