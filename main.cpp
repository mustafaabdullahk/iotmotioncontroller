/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "dac8352.h"
#include "ads1256.h"
#include <cstdio>

#define MAXIMUM_BUFFER_SIZE                                                  8

// Create a DigitalOutput object to toggle an LED whenever data is sended.
static DigitalOut led(LED3);
static BufferedSerial serial_port(USBTX, USBRX);

int main(void)
{
    // Set desired properties (115200-8-N-1).
    serial_port.set_baud(115200);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE] = {0};
    Analog2Digital adc;
    adc.Reset();
    adc.ConfigureADC(adc.ADS1256_GAIN_1, adc.ADS1256_30000SPS);
    //Digital2Analog dac;
    //dac.OutVoltage(0x30, 3);
    //dac.OutVoltage(0x34, 3);
    while (1) {
        if (uint32_t num = serial_port.write(buf, sizeof(buf))) {
            // Toggle the LED.
            led = !led;
            
            adc.ReadChipID();
            //adc.ConfigureADC(adc.ADS1256_GAIN_1, adc.ADS1256_30000SPS);
            //adc.GetAll();
            //printf("0 ADC = %d \r\n", adc.ADCValue[1]);
            //float temp = (adc.ADCValue[0] >> 7) * 5.0 / 0xffff;
            //printf("DAC: ", temp);
           // dac.OutVoltage(dac.channel_A, temp);
            //dac.OutVoltage(dac.channel_B, 3.3 - temp);
            // Echo the input back to the terminal.
            serial_port.write(buf, num);
        }
    }
}
