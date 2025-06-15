#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "driver/gpio.h"

// Definitions of SPI pins -> OLED communication
#define SPI_CH_HOST SPI2_HOST
#define SPI_CLOCK_HZ (8 * 1000 * 1000) // 8MHz

#define SPI_PIN_MISO -1 // Not used
#define SPI_PIN_MOSI (gpio_num_t)10
#define SPI_PIN_CLK (gpio_num_t)8
#define SPI_PIN_CS (gpio_num_t)9
#define SPI_PIN_DC (gpio_num_t)20
#define SPI_PIN_RST (gpio_num_t)21

// Definitions od I2C pins -> ADXL345 communication
#define I2C_PIN_SCL (gpio_num_t)7
#define I2C_PIN_SDA (gpio_num_t)6

// Definition of buttons pins -> system controll
#define BUTTON_PIN_1 (gpio_num_t)3
#define BUTTON_PIN_2 (gpio_num_t)4

#endif
