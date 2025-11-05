# Temperature-and-Humidity
## Project Overview
Displays the temperature and humidity on the DFRobot_RGBLCD1602, obtained from the built-in SHTC3 sensor on the ESP32C3 board

<p align="center">
  <img src="https://github.com/suhyeonk03/Temperature-and-Humidity/blob/main/Project_Image.jpg" alt="LCD and ESP board" width="600" height="500">
</p>

## Used Libraries
- <stdio.h>
- <string.h>
- <math.h>
- "freertos/FreeRTOS.h"
- "freertos/task.h"
- "driver/i2c.h"
- "DFRobot_RGBLCD1602.h"
- "esp_log.h"
- "esp_system.h"

## Hardware
- ESP32-C3-DevKit-RUST-1
- DFRobot_RGBLCD1602
- Breadboard, wires, female header pins, and male header pins

## Phase 1: Soldering
Soldered female header pins onto the ESP32 board using an AOYUE 9378 soldering station.

## Phase 2: Temperature and Humidity read
Connects to the SHTC3 sensor at address 0x70 and controls its power using the wake-up command (0x3517) and the sleep command (0xB098). The temperature and humidity functions each read 3 bytes using the i2c_master_write_to_device and i2c_master_read_from_device functions. After verifying data validity through a checksum and converting the 16-bit values into readable numbers, the program outputs the temperature in Celsius and humidity as a percentage.

## Phase 3: Display result on LCD (detailed expl, what act code does)
Modified the Arduino-based functions in DFRobot_RGBLCD1602.cpp to support the ESP32 interface. Then, initialized the LCD in main.cpp with a black backlight and set the cursor to position (0, 0). The LCD displays the current temperature and humidity obtained from Phase 2 in the format: "Temp:%dC,Hum:%d%%".

## Credits
- DFRobot_RGBLCD1602.cpp and DFRobot_RGBLCD1602.h — © 2010 DFRobot Co., Ltd.
- main.cpp — © 2024 Espressif Systems (Shanghai) Co., Ltd.
(Adapted from espressif/esp-idf/examples/peripherals/i2c/i2c_basic/main)
