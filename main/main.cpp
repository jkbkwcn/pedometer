#include "board_config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "menu.hpp"
#include "oled_sh1106.h"
#include "sdkconfig.h"
#include <stdio.h>

using namespace pedometer;

// Static variables for SPI and system parameters
static spi_device_handle_t spi;
static ext_spi_handle_t ext_spi;

extern "C" void app_main(void) {

  printf("Entering app_main!\n");

  // ######################### INITIALIZATION BEGINS #########################
  /* Configure OLED and related SPI */
  esp_err_t ret;

  spi_bus_config_t buscfg;
  buscfg.miso_io_num = SPI_PIN_MISO;
  buscfg.mosi_io_num = SPI_PIN_MOSI;
  buscfg.sclk_io_num = SPI_PIN_CLK;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;
  buscfg.data4_io_num = -1;
  buscfg.data5_io_num = -1;
  buscfg.data6_io_num = -1;
  buscfg.data7_io_num = -1;
  buscfg.data_io_default_level = 0;

  // spi_bus_config_t buscfg = {.miso_io_num = SPI_PIN_MISO,
  //                          .mosi_io_num = SPI_PIN_MOSI,
  //                          .sclk_io_num = SPI_PIN_CLK,
  //                          .quadwp_io_num = -1,
  //                          .quadhd_io_num = -1,
  //                          .data4_io_num = -1,
  //                          .data5_io_num = -1,
  //                          .data6_io_num = -1,
  //                          .data7_io_num = -1,
  //                          .data_io_default_level = 0};

  spi_device_interface_config_t devcfg;
  devcfg.clock_speed_hz = SPI_CLOCK_HZ;
  devcfg.mode = 0;
  devcfg.spics_io_num = SPI_PIN_CS;
  devcfg.queue_size = 7;

  // spi_device_interface_config_t devcfg = {.clock_speed_hz = SPI_CLOCK_HZ, .mode = 0, .spics_io_num = SPI_PIN_CS, .queue_size = 7};

  // Initialize the SPI bus
  ret = spi_bus_initialize(SPI_CH_HOST, &buscfg, SPI_DMA_CH_AUTO);
  ESP_ERROR_CHECK(ret);

  // Attach the OLED to the SPI bus
  ret = spi_bus_add_device(SPI_CH_HOST, &devcfg, &spi);
  ESP_ERROR_CHECK(ret);

  // Initialize SPI GPIOs not related to spi_handle_t instance
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = ((1ULL << SPI_PIN_DC) | (1ULL << SPI_PIN_RST));
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);

  // Assigning fields of extended SPI bus that contains of SPI bus, D/C and RES pins
  ext_spi.spi = spi;
  ext_spi.dc = SPI_PIN_DC;
  ext_spi.res = SPI_PIN_RST;

  // Initialize the LCD
  oled_init(ext_spi);
  // Menu::GetInstance().init(ext_spi);

  // ########################## INITIALIZATION ENDS ##########################

  while(1) {
  }
}
