/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* i2c - Simple Example
   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_system.h"
#include <math.h>
#include "DFRobot_RGBLCD1602.h"
//#include "main.h"
//#include "i2c_init.h"

static const char *TAG = "example";

#define I2C_MASTER_SCL_IO           8                           /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           10                          /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0                   /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          40000                       /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define MPU9250_SENSOR_ADDR         0x68        /*!< Address of the MPU9250 sensor */
#define MPU9250_WHO_AM_I_REG_ADDR   0x75        /*!< Register addresses of the "who am I" register */
#define MPU9250_PWR_MGMT_1_REG_ADDR 0x6B        /*!< Register addresses of the power management register */
#define MPU9250_RESET_BIT           7

#define SENSOR_ADDR 0x70
#define RGB_ADDR 0x2D
#define LCD_ADDR 0x3E

/**
 * @brief Read a sequence of bytes from a MPU9250 sensor registers
 */
static esp_err_t mpu9250_register_read(uint8_t *data, size_t len)
{
    return i2c_master_read_from_device(I2C_MASTER_NUM, SENSOR_ADDR, data, len, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Write a byte to a MPU9250 sensor register
 */
static esp_err_t mpu9250_register_write_byte(uint8_t data)
{
    uint8_t write_buf[2] = {SENSOR_ADDR, data};
    return i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS);

}

/**
 * @brief i2c master initialization
 */

/*static void i2c_master_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_master_bus_config_t bus_config = {};
        bus_config.i2c_port = I2C_MASTER_NUM;
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

    i2c_device_config_t dev_config = {};
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SENSOR_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle));
}*/

uint8_t checksum(uint8_t *data, size_t len) {
	uint8_t crc = 0xFF;
	for (size_t i=0; i<len;i++) {
		crc ^= data[i];
		for (uint8_t bit=0;bit<8;bit++) {
			if (crc&0x80) {
	         		crc = (crc << 1) ^0x31; }
			else {
				crc <<= 1; }
		}
	}
	return crc;
}

uint16_t read_from_SHT3_temp() {
	//uint8_t data[6];
	uint8_t data[3];
    uint8_t cmd[] = {0x78, 0x66};
	ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, cmd, sizeof(cmd), I2C_MASTER_TIMEOUT_MS));
	vTaskDelay(pdMS_TO_TICKS(50));
	ESP_ERROR_CHECK(i2c_master_read_from_device(I2C_MASTER_NUM, SENSOR_ADDR, data, 3, I2C_MASTER_TIMEOUT_MS));
	if (checksum(&data[0], 2) != data[2]) {
		ESP_LOGE(TAG, "Temp checksum error");
		return 0xFFFF;
	}
	
	uint16_t temp_raw = (data[0] << 8) | data[1];
	float temp_c = -45+175*((float)temp_raw/65535.0f);
	return (uint16_t)temp_c;
}

uint16_t read_from_SHT3_humi() {
	uint8_t data[3];
	uint8_t cmd[] = {0x58, 0xE0};
	ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, cmd, sizeof(cmd), I2C_MASTER_TIMEOUT_MS));
	vTaskDelay(pdMS_TO_TICKS(50));
	ESP_ERROR_CHECK(i2c_master_read_from_device(I2C_MASTER_NUM, SENSOR_ADDR, data, 3, I2C_MASTER_TIMEOUT_MS));
	if (checksum(&data[0], 2) != data[2]) {
		ESP_LOGE(TAG, "Humidity checksum error");
		return 0xFFFF;
	}
	uint16_t hum_raw = (data[0] << 8) | data[1];
	float humd = 100*((float)hum_raw/65535.0f);
	return (uint16_t)humd;
}

esp_err_t sensor_power_up() {
	uint8_t cmd[] = {0x35, 0x17};
	//return i2c_master_transmit(dev_handle, cmd, sizeof(cmd), 1000/portTICK_PERIOD_MS);
	return i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, cmd, sizeof(cmd), I2C_MASTER_TIMEOUT_MS);

}

esp_err_t sensor_power_down() {
	uint8_t cmd[] = {0xB0, 0x98};
	//return i2c_master_transmit(dev_handle, cmd, sizeof(cmd), 1000/portTICK_PERIOD_MS);
	return i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, cmd, sizeof(cmd), I2C_MASTER_TIMEOUT_MS);
}

extern "C" void app_main(void)
{
    //i2c_master_bus_handle_t bus_handle;
    //i2c_master_dev_handle_t dev_handle;

    //i2c_master_init(&bus_handle, &dev_handle);

    DFRobot_RGBLCD1602 lcd(RGB_ADDR, 16, 2, LCD_ADDR);
    lcd.init();
    lcd.setBacklight(1);
    lcd.clear();
    lcd.setRGB(255,255,255);

    while(true) {
		ESP_ERROR_CHECK(sensor_power_up());
		vTaskDelay(pdMS_TO_TICKS(10));
		uint16_t temp = read_from_SHT3_temp();
		uint16_t humidity = read_from_SHT3_humi();
		ESP_ERROR_CHECK(sensor_power_down());

		float tempf = (temp*9.0/5.0)+32.0;
		int rtemp = round(temp);
		int rtempf = round(tempf);
		int rhum = round(humidity);
		
		lcd.setCursor(0,0);
		char str[30];
		sprintf(str, "Temp:%dC,Hum:%d%%", rtemp, rhum);	
		lcd.print(str);

		vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
