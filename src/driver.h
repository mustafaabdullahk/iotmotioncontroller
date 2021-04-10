#ifndef MBED_DRIVER_H
#define MBED_DRIVER_H

#include "mbed.h"
#include "hal/spi_api.h"

class Config 
{
public:
    Config();
    int WriteByte(int value);
    char* WriteBytes(char* txData, int txSize, char* rxData, int rxSize);
};

#endif