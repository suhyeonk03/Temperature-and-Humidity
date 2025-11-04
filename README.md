# Temperature-and-Humidity
## Project Overview
Displays the temperature and humidity on the DFRobot_RGBLCD1602, obtained from the built-in SHTC3 sensor on the ESP32C3 board

<p align="center">
  <img src="https://github.com/suhyeonk03/Temperature-and-Humidity/blob/main/Project_Image.jpg" alt="LCD and ESP board" width="600" height="500">
</p>

### Credentials
- DFRobot_RGBLCD1602.cpp and DFRobot_RGBLCD1602.h are by 2010 DFRobot Co.Ltd
- main.cpp is by 2024 Espressif Systems (Shanghai) CO LTD, espressif/esp-idf/examples/peripherals/i2c/i2c_basic/main

## Used Libraries
## Hardware
- ESP32 robust board
- 

## Phase 1: Soldering
I soldered the female header onto the ESP32-C3 board. Everything worked well, except I accidentally flipped the female header
## Phase 2: Temperature and Humidity read
The temperature and Humidity functions read 3 bytes using the i2c_master functions. After checking if the value is valid using a checksum, it prints the Temperature and Humidity to the terminal. 

I spent most of the time fixing an error I got while idf.py monitor. I could fix this error by modifying the number of bytes temperature and humidity functions read at a time, from 6 bytes to 3 bytes
## Phase 3: Display result on LCD (detailed expl, what act code does)
I had to display “Hello CSE121!” on the first row and my last name on the second row. To make
this work, I modified some functions — removing print and replacing wire.h - so that the code
would work with the ESP32-C3 instead of Arduino.
At first, I had a hard time understanding which functions were specific to Arduino and which
ones I needed to replace. I spent most of my time trying to figure out why the second row
wouldn’t display. I still couldn’t fix it, and the TA suggested that I turn it in as is, since it might be
a small mistake in my code conversion that’s hard to track down

The built-in SHTC3 sensor reads temperature and humidity, and the LCD monitor displays their
values every second. I was able to implement this feature easily because it combines concepts
from lab3_2 and a previous lab. The only errors I encountered were related to ESP_LOGE.
These occurred because I was using two different libraries, i2c_master.h and i2c.h. I resolved
the issue by using only one library, i2c.h, and converting all function calls from new_version to
old_version. For example, i2c_master_transmit to i2c_master_write_to_device.
I also had to ignore spacing between characters because my second row still doesn’t work
properly
