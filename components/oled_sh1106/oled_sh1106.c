#include "oled_sh1106.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "oled_sh1106_commands.h"
#include "oled_sh1106_config.h"
#include "oled_sh1106_font.h"
#include "sdkconfig.h"
#include <assert.h>
#include <string.h>

static const oled_attributes_t oled_sh1106_attributes = {.rows = OLED_HEIGHT,
                                                         .first_row = 0x00,
                                                         .last_row = OLED_HEIGHT - 1,
                                                         .columns = OLED_WIDTH,
                                                         .first_page = 0x00,
                                                         .last_page = OLED_NUM_PAGES - 1,
                                                         .first_col = 0x00,
                                                         .last_col = OLED_WIDTH - 1,
                                                         .pages = OLED_NUM_PAGES};

// Buffer for the display
static uint8_t oled_buf[OLED_HEIGHT / 8][OLED_WIDTH];

// Command sequence for proper initialization of the OLED display
static const oled_cmd_t init_cmds_seq[] = {
    {.cmd = OLED_SLEEP_MODE_OFF, .dc = OLED_CMD},       {.cmd = OLED_DISP_OFF, .dc = OLED_CMD},
    {.cmd = OLED_SET_CLK_DIV, .dc = OLED_CMD},          {.cmd = OLED_CLK_DIV_VAL, .dc = OLED_CMD},
    {.cmd = OLED_SET_MUX_ROT, .dc = OLED_CMD},          {.cmd = OLED_MUX_ROT_VAL, .dc = OLED_CMD},
    {.cmd = OLED_SET_DISP_OFFSET, .dc = OLED_CMD},      {.cmd = OLED_DISP_OFFSET, .dc = OLED_CMD},
    {.cmd = OLED_BASE_DISP_START_LINE, .dc = OLED_CMD}, {.cmd = OLED_SET_DC_DC, .dc = OLED_CMD},
    {.cmd = OLED_ENABLE_DC_DC, .dc = OLED_CMD},         {.cmd = OLED_REMAP_NORMAL, .dc = OLED_CMD},
    {.cmd = OLED_NORM_SCAN_DIR, .dc = OLED_CMD},        {.cmd = OLED_SET_COM_PINS, .dc = OLED_CMD},
    {.cmd = OLED_COM_PINS_VAL, .dc = OLED_CMD},         {.cmd = OLED_SET_CONTRAST, .dc = OLED_CMD},
    {.cmd = OLED_CONTRAST_VAL, .dc = OLED_CMD},         {.cmd = OLED_SET_PRECHARGE, .dc = OLED_CMD},
    {.cmd = OLED_PRECHARGE_VAL, .dc = OLED_CMD},        {.cmd = OLED_SET_VCOM_DESEL, .dc = OLED_CMD},
    {.cmd = OLED_VCOM_DESEL_VAL, .dc = OLED_CMD},       {.cmd = OLED_BASE_CHARGEP_VAL, .dc = OLED_CMD},
    {.cmd = OLED_DISPLAY_POSITIVE, .dc = OLED_CMD},
};

oled_attributes_t oled_get_attributes(void) { return oled_sh1106_attributes; }

void oled_clear_buf(void) {
  for(uint8_t page = oled_sh1106_attributes.first_page; page <= oled_sh1106_attributes.last_page; page++) {
    memset(oled_buf[page], 0x00, oled_sh1106_attributes.columns);
  }
}

void oled_send_cmd(ext_spi_handle_t ext_spi, const oled_cmd_t oled_cmd) {
  esp_err_t ret;
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));
  t.length = 8;
  t.tx_buffer = &(oled_cmd.cmd);
  ret = gpio_set_level(ext_spi.dc, oled_cmd.dc);
  ESP_ERROR_CHECK(ret);
  ret = spi_device_polling_transmit(ext_spi.spi, &t);
  ESP_ERROR_CHECK(ret);
}

void oled_send_cmd_list(ext_spi_handle_t ext_spi, oled_cmd_t const *commands) {
  for(int i = 0; i < sizeof(commands); i++) {
    oled_send_cmd(ext_spi, commands[i]);
  }
}

void oled_init(ext_spi_handle_t ext_spi) {
  // Display hardware reset
  esp_err_t ret;
  ret = gpio_set_level(ext_spi.res, 0);
  ESP_ERROR_CHECK(ret);
  vTaskDelay(pdMS_TO_TICKS(100));
  ret = gpio_set_level(ext_spi.res, 1);
  ESP_ERROR_CHECK(ret);
  vTaskDelay(pdMS_TO_TICKS(100));
  // Send initialization commands, clear buffer and render empty display
  oled_send_cmd_list(ext_spi, init_cmds_seq);
  oled_clear_buf();
  oled_put_string("PEDOMETER", 4, 10, OLED_BASIC_FONT_ID);
  oled_render(ext_spi);
  vTaskDelay(pdMS_TO_TICKS(5000));
}

void oled_cursor_allign_left(ext_spi_handle_t ext_spi) {
  oled_cmd_t cmd;
  cmd.cmd = OLED_BASE_COL_ADDR_H;
  cmd.dc = OLED_CMD;
  oled_send_cmd(ext_spi, cmd);
  cmd.cmd = OLED_BASE_COL_ADDR_L;
  cmd.dc = OLED_CMD;
  oled_send_cmd(ext_spi, cmd);
}

void oled_set_page(ext_spi_handle_t ext_spi, uint8_t page) {
  assert(page >= oled_sh1106_attributes.first_page && page <= oled_sh1106_attributes.last_page);
  oled_cmd_t cmd;
  cmd.cmd = OLED_FIRST_PAGE_ADDR + page;
  cmd.dc = OLED_CMD;
  oled_send_cmd(ext_spi, cmd);
}

void oled_render(ext_spi_handle_t ext_spi) {
  oled_cmd_t cmd;
  for(uint8_t page = oled_sh1106_attributes.first_page; page <= oled_sh1106_attributes.last_page; page++) {
    oled_cursor_allign_left(ext_spi);
    oled_set_page(ext_spi, page);
    for(uint8_t col = oled_sh1106_attributes.first_col; col <= oled_sh1106_attributes.last_col; col++) {
      cmd.cmd = oled_buf[page][col];
      cmd.dc = OLED_DATA;
      oled_send_cmd(ext_spi, cmd);
    }
  }
}

void oled_clear_display(ext_spi_handle_t ext_spi) {
  oled_clear_buf();
  oled_render(ext_spi);
}

void oled_put_point(oled_point_t point) {
  assert(oled_sh1106_attributes.first_col <= point.x && oled_sh1106_attributes.last_col >= point.x);
  assert(oled_sh1106_attributes.first_row <= point.y && oled_sh1106_attributes.last_row >= point.y);
  uint8_t page = point.y / 8;
  uint8_t bit = 1 << (point.y % 8);
  if(OLED_COLOR_WHITE == point.color) {
    oled_buf[page][point.x] |= bit;
  } else {
    oled_buf[page][point.x] &= ~bit;
  }
}

void oled_put_vline(oled_line_t line) {
  assert(oled_sh1106_attributes.first_col <= line.orig_x && oled_sh1106_attributes.last_col >= line.orig_x);
  assert(oled_sh1106_attributes.first_row <= line.orig_y && oled_sh1106_attributes.last_row >= (line.orig_y + line.len - 1));
  for(uint8_t pt = 0; pt < line.len; pt++) {
    oled_put_point((oled_point_t){.x = line.orig_x, .y = line.orig_y + pt, .color = line.color});
  }
}

void oled_put_hline(oled_line_t line) {
  assert(oled_sh1106_attributes.first_col <= line.orig_x && oled_sh1106_attributes.last_col >= (line.orig_x + line.len - 1));
  assert(oled_sh1106_attributes.first_row <= line.orig_y && oled_sh1106_attributes.last_row >= line.orig_y);
  for(uint8_t pt = 0; pt < line.len; pt++) {
    oled_put_point((oled_point_t){.x = line.orig_x + pt, .y = line.orig_y, .color = line.color});
  }
}

void oled_put_rect(oled_rect_t rect) {
  assert(oled_sh1106_attributes.first_col <= rect.orig_x && oled_sh1106_attributes.last_col >= rect.orig_x);
  assert(oled_sh1106_attributes.first_col <= rect.end_x && oled_sh1106_attributes.last_col >= rect.end_x);
  assert(oled_sh1106_attributes.first_row <= rect.orig_y && oled_sh1106_attributes.last_row >= rect.orig_y);
  assert(oled_sh1106_attributes.first_row <= rect.end_y && oled_sh1106_attributes.last_row >= rect.end_y);
  if(OLED_SHAPE_FILLED == rect.fill) {
    // Fill the rectangle with points
    for(uint8_t col = rect.orig_x; col <= rect.end_x; col++) {
      for(uint8_t row = rect.orig_y; row <= rect.end_y; row++) {
        oled_put_point((oled_point_t){.x = col, .y = row, .color = rect.color});
      }
    }
  } else {
    // Drawing border
    // Draw horizontal border lines
    oled_put_hline((oled_line_t){.orig_x = rect.orig_x, .orig_y = rect.orig_y, .len = rect.end_x - rect.orig_x + 1, .color = rect.color});
    oled_put_hline((oled_line_t){.orig_x = rect.orig_x, .orig_y = rect.end_y, .len = rect.end_x - rect.orig_x + 1, .color = rect.color});
    // Draw vertical border lines
    oled_put_vline((oled_line_t){.orig_x = rect.orig_x, .orig_y = rect.orig_y, .len = rect.end_y - rect.orig_y + 1, .color = rect.color});
    oled_put_vline((oled_line_t){.orig_x = rect.end_x, .orig_y = rect.orig_y, .len = rect.end_y - rect.orig_y + 1, .color = rect.color});
  }
}

void oled_put_basic_font_char(const char c, uint8_t page, uint8_t col) {
  assert(oled_sh1106_attributes.first_col <= col && oled_sh1106_attributes.last_col >= col + OLED_BASIC_FONT_COLS - 1);
  assert(oled_sh1106_attributes.first_page <= page && oled_sh1106_attributes.last_page >= page);
  uint8_t char_index = 0;
  for(uint8_t c_idx = 0; c_idx < sizeof(oled_font_characters) / sizeof(oled_font_characters[0]); c_idx++) {
    if(oled_font_characters[c_idx] == c) {
      char_index = c_idx;
      break;
    }
  }
  // Copy the character data into the buffer
  memcpy(&oled_buf[page][col], &oled_basic_font_lut[char_index], OLED_BASIC_FONT_COLS);
}

void oled_put_char(const char c, uint8_t page, uint8_t col, uint8_t font_id) {
  switch(font_id) {
  case OLED_BASIC_FONT_ID:
    oled_put_basic_font_char(c, page, col);
    break;
  default:
    break;
  }
}

void oled_put_string(const char *str, uint8_t page, uint8_t col, uint8_t font_id) {
  switch(font_id) {
  case OLED_BASIC_FONT_ID:
    assert(oled_sh1106_attributes.first_col <= col && oled_sh1106_attributes.last_col >= (col + strlen(str) * OLED_BASIC_FONT_COLS - 1));
    assert(oled_sh1106_attributes.first_page <= page && oled_sh1106_attributes.last_page >= page);
    break;
  default:
    // Unsupported font ID
    return;
  }
  for(size_t i = 0; i < strlen(str); i++) {
    oled_put_char(str[i], page, col + (i * OLED_BASIC_FONT_COLS), font_id);
  }
}

void oled_put_bitmap(const uint8_t *bitmap) {
  assert(sizeof(oled_buf) == sizeof(bitmap));
  memcpy(&oled_buf, bitmap, sizeof(oled_buf));
}

void oled_sleep(ext_spi_handle_t ext_spi) {
  oled_cmd_t cmd;
  cmd.cmd = OLED_SLEEP_MODE_ON;
  cmd.dc = OLED_CMD;
  oled_send_cmd(ext_spi, cmd);
}

void oled_wakeup(ext_spi_handle_t ext_spi) {
  oled_cmd_t cmd;
  cmd.cmd = OLED_SLEEP_MODE_OFF;
  cmd.dc = OLED_CMD;
  oled_send_cmd(ext_spi, cmd);
}
