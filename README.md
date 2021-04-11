# IOT Motion Controller

## Interface Definition

| Pin    | Symbol | Description |
| ------ | ------ | ------ |
| 1, 17  | 3.3V   | Power supply (3.3V) |
| 2      | 5V     | Power supply (5V) |
| 3, 5, 7, 8, 10, 18, 22, 24, 26, 27, 28, 29, 32, 36, 38, 40  | NC | NC |
| 6, 9, 14, 20, 25, 30, 34, 39  | GND | Ground |
| 11     | DRDY   | ADS1256 data ready output, low active |
| 12     | RESET  | ADS1256 reset input |
| 13     | PWDN   | ADS1256 sync/power off input, low active |
| 15     | CS0    | ADS1256 chip select, low active |
| 16     | CS1    | DAC8532 chip select, low active |
| 19     | DIN    | SPI data input |
| 21     | DOUT   | SPI data output |
| 23     | SCK    | SPI clock |
| 31     | GPIO   | extend to sensor interface |


## Application functionality



## Building and running

1. Connect a USB cable between the USB port on the board and the host computer.
1. Run the following command to build the example project and program the microcontroller flash memory:

    * Mbed CLI 2

    ```bash
    $ mbed-tools compile -m <TARGET> -t <TOOLCHAIN> --flash
    ```

    * Mbed CLI 1

    ```bash
    $ mbed compile -m <TARGET> -t <TOOLCHAIN> --flash
    ```

Your PC may take a few minutes to compile your code.

Alternatively, you can manually copy the binary to the board, which you mount on the host computer over USB.

## Expected output

