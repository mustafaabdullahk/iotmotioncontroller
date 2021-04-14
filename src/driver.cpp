#include "src/driver.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS); //chip select

Config::Config()
{
    spi.format(8, 1);
    spi.frequency(20000);
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
    printf("send value %x", txData[0]);
    printf("send value %x", txData[1]);
    printf("value %x", rxData[0]);
    printf("value %x", rxData[1]);
    printf("value %x", rxData[2]);
    printf("value %x", rxData[3]);
    printf("value %x", rxData[4]);
    printf("value %x", rxData[5]);
    uint8_t ttt;
    memcpy(&ttt, rxData, sizeof(rxData));
    printf("value totals %x \n", ttt);
    //printf("stringggggggggg %s", rxData);
    printf("testtttttttt %x \n", test);
    return rxData;
}