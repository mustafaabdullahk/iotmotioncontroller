#ifndef MBED_ANALOG2DIGITAL_H
#define MBED_ANALOG2DIGITAL_H

/*Status register define
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)
			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.
			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.
			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled
			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.
			ACAL=1  enable  calibration
*/

#include <cstdint>
#include <utility>
#include "src/driver.h"

class Analog2Digital{

public:
    enum ADS1256_GAIN_E{
        ADS1256_GAIN_1  = 0, // GAIN   1
        ADS1256_GAIN_2  = 1, // GAIN   2
        ADS1256_GAIN_4  = 2, // GAIN   4
        ADS1256_GAIN_8  = 3, // GAIN   8
        ADS1256_GAIN_16 = 4, // GAIN  16
        ADS1256_GAIN_32 = 5, // GAIN  32
        ADS1256_GAIN_64 = 6, // GAIN  64
    };
    enum ADS1256_DRATE_E{
        ADS1256_30000SPS = 0xF0, // reset the default values
        ADS1256_15000SPS = 0xE0,
        ADS1256_7500SPS  = 0xD0,
        ADS1256_3750SPS  = 0xC0,
        ADS1256_2000SPS  = 0xB0,
        ADS1256_1000SPS  = 0xA1,
        ADS1256_500SPS   = 0x92,
        ADS1256_100SPS   = 0x82,
        ADS1256_60SPS    = 0x72,
        ADS1256_50SPS    = 0x63,
        ADS1256_30SPS    = 0x53,
        ADS1256_25SPS    = 0x43,
        ADS1256_15SPS    = 0x33,
        ADS1256_10SPS    = 0x20,
        ADS1256_5SPS     = 0x13,
        ADS1256_2d5SPS   = 0x03
    };
    enum REG_E{
        REG_STATUS  = 0,  // x1H
        REG_MUX     = 1,  // 01H
        REG_ADCON   = 2,  // 20H
        REG_DRATE   = 3,  // F0H
        REG_IO      = 4,  // E0H
        REG_OFC0    = 5,    // xxH
        REG_OFC1    = 6,    // xxH
        REG_OFC2    = 7,    // xxH
        REG_FSC0    = 8,    // xxH
        REG_FSC1    = 9,    // xxH
        REG_FSC2    = 10,   // xxH
    };
    enum CMD{
        CMD_WAKEUP   = 0x00,     // Completes SYNC and Exits Standby Mode 0000  0000 (00h)
        CMD_RDATA    = 0x01,      // Read Data 0000  0001 (01h)
        CMD_RDATAC   = 0x03,     // Read Data Continuously 0000   0011 (03h)
        CMD_SDATAC   = 0x0F,     // Stop Read Data Continuously 0000   1111 (0Fh)
        CMD_RREG     = 0x10,       // Read from REG rrr 0001 rrrr (1xh)
        CMD_WREG     = 0x50,       // Write to REG rrr 0101 rrrr (5xh)
        CMD_SELFCAL  = 0xF0,    // Offset and Gain Self-Calibration 1111    0000 (F0h)
        CMD_SELFOCAL = 0xF1,   // Offset Self-Calibration 1111    0001 (F1h)
        CMD_SELFGCAL = 0xF2,   // Gain Self-Calibration 1111    0010 (F2h)
        CMD_SYSOCAL  = 0xF3,    // System Offset Calibration 1111   0011 (F3h)
        CMD_SYSGCAL  = 0xF4,    // System Gain Calibration 1111    0100 (F4h)
        CMD_SYNC     = 0xFC,       // Synchronize the A/D Conversion 1111   1100 (FCh)
        CMD_STANDBY  = 0xFD,    // Begin Standby Mode 1111   1101 (FDh)
        CMD_RESET    = 0xFE,      // Reset to Power-Up Values 1111   1110 (FEh)
    };

public:
    Analog2Digital();
    int Reset();
    int WriteCommand(uint8_t reg);
    int WriteRegister(uint8_t reg, uint16_t data);
    char* ReadData(uint8_t reg);
    int WaitDRDY();
    int ReadChipID();
    int ConfigureADC(unsigned char gain, unsigned char drate);
    int SetChannel(int chn);
    int SetDiffChannel(int chn);
    void SetMode(char mode);
    int ReadADCData();
    int GetChannelValue(int Channel);
    void GetAll();
private:
    Config spiHandle;
    int ScanMode = 0;
public:
    int ADCValue[8] = {0,0,0,0,0,0,0,0};
};
#endif