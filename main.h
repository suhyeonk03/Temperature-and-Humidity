#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_system.h"
#include <math.h>

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

uint16_t read_from_SHT3_temp();
uint16_t read_from_SHT3_humi();
esp_err_t sensor_power_up();
esp_err_t sensor_power_down();
extern "C" void app_main(void);
uint8_t checksum(uint8_t *data, size_t len);
