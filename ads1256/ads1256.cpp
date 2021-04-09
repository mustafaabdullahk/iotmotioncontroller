#include "ads1256.h"

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS);

Analog2Digital::Analog2Digital()
{

}

int Analog2Digital::Reset()
{
    return 0;
}

int Analog2Digital::WriteCommand(char reg)
{
    cs = false;
    if(spi.write(reg))
    {
        cs = true;
        return 1;
    }
    return 0;
}

int Analog2Digital::WriteRegister(char reg, char data)
{
    cs = false;
    char *txData;
    txData[0] = CMD_WREG | reg;
    txData[1] = 0x00;
    txData[2] = data;
    char rxData;
    if(spi.write(txData,sizeof(txData), &rxData, sizeof(rxData)))
    {
        cs = true;
        return 1;
    }
    return 0;
}

int Analog2Digital::ReadData(char reg)
{
    cs = false;
    char *txData;
    txData[0] = CMD_RREG | reg;
    txData[1] = 0x00;
    char rxData;
    if(spi.write(txData,sizeof(txData), &rxData, sizeof(rxData)))
    {
        cs = true;
        return rxData;
    }
    return 0;
}