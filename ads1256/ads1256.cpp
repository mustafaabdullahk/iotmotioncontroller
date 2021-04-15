#include "ads1256.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#define DELAY 100


DigitalIn drdy(D8); //ADS1256 data ready output, low active
DigitalOut reset(D7); //ADS1256 reset input

Analog2Digital::Analog2Digital()
{
}

int Analog2Digital::Reset()
{
    reset = 1;
    wait_us(DELAY * 1000);
    reset = 0;
    wait_us(DELAY * 1000);
    reset = 1;
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
    char rxData;
    auto data = spiHandle.WriteBytes(str.c_str(),str.size(), &rxData, sizeof(rxData));
    printf("Chip ID %u \r\n", data[0]);
    if(data != nullptr)
    {
        return data;
    }
    return nullptr;
}
int Analog2Digital::WaitDRDY()
{
    wait_us(DELAY * 1000);
      auto state = drdy.read(); 
      if (state != 0) {
        printf("Timeout ...");
      } else {
        return 1;
    }
    return 0;
}

int Analog2Digital::ReadChipID()
{
    WaitDRDY();
    char *id = ReadData(REG_STATUS);
    char chipID = id[0] >> 4;
    //printf("Chip ID %c", chipID);
    return 0;
}

int Analog2Digital::ConfigureADC(unsigned char gain, unsigned char drate)
{
    WaitDRDY();
    char *buf;
    std::string str;
    std::string cmd;
    str.push_back((0 << 3) | (1 << 2) | (0 << 1));
    str.push_back(0x08);
    str.push_back((0 << 5) | (0 << 3) | (gain << 0));
    str.push_back(drate);
    str.push_back(0x00);
    str.push_back(0x00);
    str.push_back(0x00);
    str.push_back(0x00);
    cmd.push_back(CMD_WREG | 0);
    cmd.push_back(0x03);
    char rxData;
    spiHandle.WriteBytes(cmd.c_str(), str.size(), &rxData, sizeof(rxData));
    spiHandle.WriteBytes(str.c_str(), str.size(), &rxData, sizeof(rxData));
    return 0;
}

int Analog2Digital::SetChannel(int chn)
{
    if (chn > 7) {
        return 0;
    }
    else {
        std::string str;
        str.push_back(REG_MUX);
        str.push_back((chn << 4) | (1 << 3));
        char rxData;
        spiHandle.WriteBytes(str.c_str(), str.size(), &rxData, sizeof(rxData));
    }
    return 0;
}

int Analog2Digital::SetDiffChannel(int chn)
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
    //spiHandle.WriteByte(static_cast<int>(CMD_RDATA));
    std::string str = std::to_string(CMD_RDATA);
    char rxData;
    //printf("txdata %s", str.c_str());
    auto data = spiHandle.WriteBytes(str.c_str(), str.size(), &rxData, 3);
    uint32_t value = 0;
    //printf("Rxdata %u", sizeof(rxData));
    memcpy(&value, data, sizeof(data));
    //printf("value %i", value);
    value = (data[0] << 16) & 0xff0000;
    value += (data[1] << 8) & 0xff00;
    value += data[2] & 0xff;
    
    if(value & 0x800000)
    {
        value &= 0xf000000;
        return value;
    }
    return 0;
}

int Analog2Digital::GetChannelValue(int Channel)
{
    int value = 0;
    if (ScanMode == 0) {
      if (Channel >= 8) {
        return 0;
      }
      SetChannel(Channel);
      WriteCommand(CMD_SYNC);
      WriteCommand(CMD_WAKEUP);
      value = ReadADCData();
    } else {
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