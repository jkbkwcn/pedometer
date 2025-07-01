#include "board_config.h"
#include "clock_counter.hpp"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "menu.hpp"
#include "oled_sh1106.h"
#include "sdkconfig.h"
#include "system_data.hpp"
#include <atomic>
#include <stdio.h>

using namespace pedometer;

enum : uint64_t { TIMER_IRQ_PERIOD = 10000 };

// Static variables for SPI and system parameters
static spi_device_handle_t spi;
static ext_spi_handle_t ext_spi;

// Interrupt flags
std::atomic<bool> tickCountFlag = false;

// Callbacks
static void clock_counter_callback(void *args) { tickCountFlag.store(true); }

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
  ;

  spi_device_interface_config_t devcfg;
  devcfg.clock_speed_hz = SPI_CLOCK_HZ;
  devcfg.mode = 0;
  devcfg.spics_io_num = SPI_PIN_CS;
  devcfg.queue_size = 7;

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

  // Timer initialization
  const esp_timer_create_args_t clock_counter_args = {.callback = &clock_counter_callback, .name = "system_clock"};

  esp_timer_handle_t clock_counter;
  ESP_ERROR_CHECK(esp_timer_create(&clock_counter_args, &clock_counter));

  // Start the timers
  ESP_ERROR_CHECK(esp_timer_start_periodic(clock_counter, TIMER_IRQ_PERIOD));

  // SystemData initialization
  SystemData::GetInstance()->init();

  // ClockCounter initialization
  ClockCounter::GetInstance()->init();

  // OLED initialization
  oled_init(ext_spi);

  // Menu instance initialization
  Menu::GetInstance().init(ext_spi);

  // ########################## INITIALIZATION ENDS ##########################

  while(1) {
    if(tickCountFlag) {
      tickCountFlag.store(false);
      ClockCounter::GetInstance()->processTick();
    }
  }
}
