#include "ads1256.h"
#include <cstdint>
#include <cstdio>
#include <string>

#define DELAY 100


DigitalOut drdy(D8); //ADS1256 data ready output, low active
DigitalOut reset(D7); //ADS1256 reset input

Analog2Digital::Analog2Digital()
{

}

int Analog2Digital::Reset()
{
    reset = true;
    wait_us(DELAY * 1000);
    reset = false;
    wait_us(DELAY * 1000);
    reset = true;
    return 0;
}

int Analog2Digital::WriteCommand(uint8_t reg)
{
    spiHandle.WriteByte(static_cast<int>(reg));
    return 0;
}

int Analog2Digital::WriteRegister(uint8_t reg, uint16_t data)
{
    std::string str;
    str.push_back(CMD_WREG | reg);
    str.push_back(0x00);
    str.push_back(data);
    char rxData = 0;
    if(spiHandle.WriteBytes(str.c_str(),sizeof(str.size()), &rxData, sizeof(rxData)) != nullptr)
    {
        return 1;
    }
    return 0;
}

char* Analog2Digital::ReadData(uint8_t reg)
{
    std::string str;
    str.push_back(CMD_RREG | reg);
    str.push_back(0x00);
    char *rxData = nullptr;
    if(spiHandle.WriteBytes(str.c_str(),str.size(), rxData, sizeof(rxData)) != nullptr)
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

int Analog2Digital::SetChannel(uint8_t chn)
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
        std::string str;
        str.push_back(REG_MUX);
        str.push_back((0 << 4) | 1);//DiffChannal  AIN0-AIN1
        char *rxData;
        spiHandle.WriteBytes(str.c_str(), str.size(), rxData, sizeof(rxData));
    }
    if (chn == 1) {
        std::string str;
        str.push_back(REG_MUX);
        str.push_back((2 << 4) | 3); //DiffChannal  AIN2-AIN3
        char *rxData;
        spiHandle.WriteBytes(str.c_str(), str.size(), rxData, sizeof(rxData));
    }
    if (chn == 2) {
        std::string str;
        str.push_back(REG_MUX);
        str.push_back((4 << 4) | 5); //DiffChannal  AIN4-AIN5
        char *rxData;
        spiHandle.WriteBytes(str.c_str(), str.size(), rxData, sizeof(rxData));
    }
    if (chn == 3) {
        std::string str;
        str.push_back(REG_MUX);
        str.push_back((6 << 4) | 7); //DiffChannal  AIN6-AIN7
        char *rxData;
        spiHandle.WriteBytes(str.c_str(), str.size(), rxData, sizeof(rxData));
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
    std::string str = std::to_string(3);
    char *rxData;
    spiHandle.WriteBytes(str.c_str(), str.size(), rxData, sizeof(rxData));
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

int Analog2Digital::GetChannelValue(uint8_t Channel)
{
    int value;
    if (ScanMode == 0) {
        if (Channel >= 0x08) {
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