#ifndef OLED_SH1106_H
#define OLED_SH1106_H

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "oled_sh1106_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Additional defines
#define OLED_UNIT_BUF_LEN OLED_WIDTH
#define OLED_STRING_OFFSET 5

enum { OLED_COLOR_BLACK = false, OLED_COLOR_WHITE = true };

enum { OLED_SHAPE_NOT_FILLED = false, OLED_SHAPE_FILLED = true };

// ---------------------------------------------------------------------------------

/**
 * @brief Stores parameters of an OLED display;
 */
typedef struct {
  uint8_t rows;
  uint8_t first_row;
  uint8_t last_row;
  uint8_t first_page;
  uint8_t last_page;
  uint8_t pages;
  uint8_t columns;
  uint8_t first_col;
  uint8_t last_col;
} oled_attributes_t;

/**
 * @brief Struct that stores pointers to SPI device handler and other required pins
 */
typedef struct {
  spi_device_handle_t spi;
  gpio_num_t dc;  // Data or command GPIO
  gpio_num_t res; // Reset
} ext_spi_handle_t;

/**
 * @brief Structure of a single command;
 */
typedef struct {
  uint8_t cmd; // Command itself
  uint8_t dc;  // Command or data type
} oled_cmd_t;

/**
 * @brief Structure representing a point - pixel;
 */
typedef struct {
  uint8_t x;  // x coordinate
  uint8_t y;  // y coordinate
  bool color; // Color -> OLED_COLOR_WHITE for white, OLED_COLOR_BLACK for black
} oled_point_t;

/**
 * @brief Structure representing a line;
 */
typedef struct {
  uint8_t orig_x; // Origin point -> x coordinate
  uint8_t orig_y; // Origin point -> y coordinate
  uint8_t len;    // Length of the line
  bool color;     // Color -> OLED_COLOR_WHITE for white, OLED_COLOR_BLACK for black; point color isn't used
} oled_line_t;

/**
 * @brief Structure representing a rectangle;
 */
typedef struct {
  uint8_t orig_x; // Origin point -> x coordinate
  uint8_t orig_y; // Origin point -> y coordinate
  uint8_t end_x;  // End point -> x coordinate
  uint8_t end_y;  // End point -> y coordinate
  bool color;     // Color -> OLED_COLOR_WHITE for white, OLED_COLOR_BLACK for black; point color isn't used
  bool fill;      // Fill -> OLED_SHAPE_FILLED for filling, OLED_SHAPE_NOT_FILLED for borders only
} oled_rect_t;

/**
 * @brief Gets the attributes of the OLED display.
 *
 * @return oled_attributes_t structure containing the attributes of the OLED display.
 */
oled_attributes_t oled_get_attributes(void);

/**
 * @brief Initialization of OLED display.
 *
 * @param ext_spi expanded SPI device handle to use for communication.
 *
 * @return void
 */
void oled_init(ext_spi_handle_t ext_spi);

/**
 * @brief Clears the display
 *
 * Clears the display by setting values of all pixels to 0x00
 *
 * @param ext_spi expanded SPI device handle to use for communication.
 *
 * @return void
 */
void oled_clear_display(ext_spi_handle_t ext_spi);

/**
 * @brief Clears the video buffer.
 *
 * Clears the video buffer by setting values of all pixels to 0x00
 *
 * @return void
 */
void oled_clear_buf(void);

/**
 * @brief Puts a point into the video buffer.
 *
 * @param point point to draw
 *
 * @return void
 *
 * @note The point is put into the video buffer, but not rendered immediately.
 */
void oled_put_point(oled_point_t point);

/**
 * @brief Puts a vertical line into the video buffer.
 *
 * @param line line with it's beginning and length
 *
 * @return void
 *
 * @note The line is put into the video buffer, but not rendered immediately.
 */
void oled_put_vline(oled_line_t line);

/**
 * @brief Puts a horizontal line into the video buffer.
 *
 * @param line line with it's beginning and length
 *
 * @return void
 *
 * @note The line is put into the video buffer, but not rendered immediately.
 */
void oled_put_hline(oled_line_t line);

/**
 * @brief Puts a rectangle into the video buffer.
 *
 * @param rect rectangle with it's beggining and end point, can be filled
 *
 * @return void
 *
 * @note The rectangle is put into the video buffer, but not rendered immediately.
 */
void oled_put_rect(oled_rect_t rect);

/**
 * @brief Puts a character at given location into the video buffer.
 *
 * @param c character
 * @param page page
 * @param col column
 * @param font_id font ID to use for the character
 *
 * @return void
 *
 * @note The character is put into the video buffer, but not rendered immediately.
 */
void oled_put_char(const char c, uint8_t page, uint8_t col, uint8_t font_id);

/**
 * @brief Puts a string at given location into the video buffer.
 *
 * @param str string
 * @param page page
 * @param col column
 * @param font_id font ID to use for the string
 *
 * @return void
 *
 * @note The string is put into the video buffer, but not rendered immediately.
 */
void oled_put_string(const char *str, uint8_t page, uint8_t col, uint8_t font_id);

/**
 * @brief Puts a given bitmap into the video buffer.
 *
 * @param bitmap bitmap to draw
 *
 * @return void
 *
 * @note The bitmap is put into the video buffer, but not rendered immediately.
 */
void oled_put_bitmap(const uint8_t *bitmap);

/**
 * @brief Renders the display by sending all data from video buffer.
 *
 * @param ext_spi expanded SPI device handle to use for communication.
 *
 * @return void
 */
void oled_render(ext_spi_handle_t ext_spi);

/**
 * @brief Turns off the display -> sleep mode enabled
 *
 * @param ext_spi expanded SPI device handle to use for communication.
 *
 * @return void
 */
void oled_sleep(ext_spi_handle_t ext_spi);

/**
 * @brief Wakes up the display -> sleep mode disabled
 *
 * @param ext_spi expanded SPI device handle to use for communication.
 *
 * @return void
 */
void oled_wakeup(ext_spi_handle_t ext_spi);

#ifdef __cplusplus
}
#endif

#endif // OLED_SH1106_H
