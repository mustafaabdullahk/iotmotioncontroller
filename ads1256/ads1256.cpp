#include "ads1256.h"
#include <cstdint>
#include <cstdio>

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
    spiHandle.WriteByte(reg);
    return 0;
}

int Analog2Digital::WriteRegister(char reg, char data)
{
    char *txData;
    txData[0] = CMD_WREG | reg;
    txData[1] = 0x00;
    txData[2] = data;
    char rxData;
    if(spiHandle.WriteBytes(txData,sizeof(txData), &rxData, sizeof(rxData)))
    {
        return 1;
    }
    return 0;
}

char* Analog2Digital::ReadData(char reg)
{
    char *txData;
    txData[0] = CMD_RREG | reg;
    txData[1] = 0x00;
    char *rxData;
    if(spiHandle.WriteBytes(txData,sizeof(txData), rxData, sizeof(rxData)))
    {
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
    char chipID = id[0] >> 4;
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
    spiHandle.WriteBytes(txdata, sizeof(txdata), rxData, sizeof(rxData));
    spiHandle.WriteBytes(buf, sizeof(buf), rxData, sizeof(rxData));
    return 0;
}

int Analog2Digital::SetChannel(char chn)
{
    if (chn > 7) {
    char *txData;
    txData[0] = REG_MUX;
    txData[1] = (chn << 4) | (1 << 3);
    char *rxData;
    spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    }
    return 0;
}

int Analog2Digital::SetDiffChannel(char chn)
{
    if (chn == 0) {
        char *txData;
        txData[0] = REG_MUX;
        txData[1] = (0 << 4) | 1; //DiffChannal  AIN0-AIN1
        char *rxData;
        spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    }
    if (chn == 1) {
        char *txData;
        txData[0] = REG_MUX;
        txData[1] = (2 << 4) | 3; //DiffChannal  AIN2-AIN3
        char *rxData;
        spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    }
    if (chn == 2) {
        char *txData;
        txData[0] = REG_MUX;
        txData[1] = (4 << 4) | 5; //DiffChannal  AIN4-AIN5
        char *rxData;
        spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    }
    if (chn == 3) {
        char *txData;
        txData[0] = REG_MUX;
        txData[1] = (6 << 4) | 7; //DiffChannal  AIN6-AIN7
        char *rxData;
        spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    }
    return 0;
}

void Analog2Digital::SetMode(char mode)
{
    ScanMode = mode;
}

int Analog2Digital::ReadADCData()
{
    WaitDRDY();
    spiHandle.WriteByte(CMD_RDATA);
    char *txData;
    txData[0] = 3;
    char *rxData;
    spiHandle.WriteBytes(txData, sizeof(txData), rxData, sizeof(rxData));
    int data;
    data = (rxData[0] << 16) & 0xff0000;
    data += (rxData[1] << 8) & 0xff00;
    data += rxData[2] & 0xff;
    if(data & 0x800000)
    {
        data &= 0xf000000;
        return data;
    }
    return 0;
}

int Analog2Digital::GetChannelValue(char Channel)
{
    int value;
    if (ScanMode == 0) {
        if (Channel >= 8) {
            return 0;
        }
        SetChannel(Channel);
        WriteCommand(CMD_SYNC);
        WriteCommand(CMD_WAKEUP);
        value = ReadADCData();
    }
    else {
        if (Channel >= 4) {
            return 0;
        }
        SetDiffChannel(Channel);
        WriteCommand(CMD_SYNC);
        WriteCommand(CMD_WAKEUP);
        value = ReadADCData();
    }
    return value;
}

void Analog2Digital::GetAll()
{
    for (int c = 0; c < 8; c++) {
        ADCValue[c] = GetChannelValue(c);
    }
}