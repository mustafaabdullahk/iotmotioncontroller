#include "ads1256.h"
#include <cstdint>
#include <cstdio>

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK); // mosi, miso, sclk
DigitalOut cs(SPI_CS);
DigitalOut drdy(D8);

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

char* Analog2Digital::ReadData(char reg)
{
    cs = false;
    char *txData;
    txData[0] = CMD_RREG | reg;
    txData[1] = 0x00;
    char *rxData;
    if(spi.write(txData,sizeof(txData), rxData, sizeof(rxData)))
    {
        cs = true;
        return rxData;
    }
    return nullptr;
}
int Analog2Digital::WaitDRDY()
{
    for (int c = 0; c < 400000; c++) 
    {
        if (drdy) {
            printf("Timeout ...");
        }
    }
    return 0;
}

int Analog2Digital::ReadChipID()
{
    WaitDRDY();
    char *id = ReadData(REG_STATUS);
    const char chipID = id[0] >> 4;
    printf("Chip ID %f", chipID);
    return 0;
}

int Analog2Digital::ConfigureADC(unsigned char gain, unsigned char drate)
{
    WaitDRDY();
    char *buf;
    buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
    buf[1] = 0x08;
    buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
    buf[3] = drate;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
    char *txdata;
    txdata[0] = CMD_WREG | 0;
    txdata[1] = 0x03;
    char *rxData;
    cs = false;
    spi.write(txdata, sizeof(txdata), rxData, sizeof(rxData));
    spi.write(buf, sizeof(buf), rxData, sizeof(rxData));
    cs = true;
    return 0;
}