#ifndef OLED_SH1106_COMMANDS_H
#define OLED_SH1106_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

// Defines if command or data is transmitted
enum { OLED_CMD = 0, OLED_DATA = 1 };

// ---------------------------- Command list ----------------------------
#define OLED_BASE_COL_ADDR_L ((uint8_t)0x02)      // 4 lower bits of address -> 0x00 - 0x0F;
#define OLED_BASE_COL_ADDR_H ((uint8_t)0x10)      // 4 higher bits of address -> 0x10 - 0x1F;
#define OLED_BASE_CHARGEP_VAL ((uint8_t)0x32)     // Specifies output voltage of the internal charge pump -> 0x30 - 0x33;
#define OLED_BASE_DISP_START_LINE ((uint8_t)0x40) // Specifies display starting line -> 0x40 - 0x7F;
#define OLED_SET_CONTRAST ((uint8_t)0x81)         // Enable constrast mode register -> MUST BE FOLLOWED BY VALUE!
#define OLED_CONTRAST_VAL ((uint8_t)0xBF)         // 0x00 - 0xFF;
#define OLED_REMAP_NORMAL ((uint8_t)0xA0)         // Normal mapping;
#define OLED_REMAP_REVERSE ((uint8_t)0xA1)        // Reversed mapping - display can be reversed by software (flips horizontally);
#define OLED_DISP_OFF ((uint8_t)0xA4)             // Turn off the display, RAM data are held;
#define OLED_DISP_ON ((uint8_t)0xA5)              // Turn on the display, RAM data are held;
#define OLED_DISPLAY_POSITIVE ((uint8_t)0xA6)     // Positive display;
#define OLED_DISPLAY_NEGATIVE ((uint8_t)0xA7)     // Negative display;
#define OLED_SET_MUX_ROT ((uint8_t)0xA8)          // Enable multiplex ratio register - MUST BE FOLLOWED BY VALUE!
#define OLED_MUX_ROT_VAL ((uint8_t)0x3F)          // 0x00 - 0x3F;
#define OLED_SET_DC_DC ((uint8_t)0xAD) // Enable/disable internal DC-DC converter. Disabling means using external supply - MUST BE FOLLOWED BY VALUE!
#define OLED_ENABLE_DC_DC ((uint8_t)0x8B)    // 0x8A -> disable; 0x8B -> enable;
#define OLED_DISABLE_DC_DC ((uint8_t)0x8A)   // 0x8A -> disable; 0x8B -> enable;
#define OLED_SLEEP_MODE_ON ((uint8_t)0xAE)   // Enable sleep mode;
#define OLED_SLEEP_MODE_OFF ((uint8_t)0xAF)  // Disable sleep mode;
#define OLED_FIRST_PAGE_ADDR ((uint8_t)0xB0) // First page address. Pages -> 0xB0 - 0xB7;
#define OLED_LAST_PAGE_ADDR ((uint8_t)0xB7)  // Last page address;
#define OLED_NORM_SCAN_DIR ((uint8_t)0xC0)   // Sets normal scan direction -> 0xC0 - 0xC8;
#define OLED_INV_SCAN_DIR ((uint8_t)0xC8)    // Sets inverted scan direction (flips vertically);
#define OLED_SET_DISP_OFFSET ((uint8_t)0xD3) // Enable display offset register - MUST BE FOLLOWED BY VALUE!
#define OLED_DISP_OFFSET ((uint8_t)0x00)     // 0x00 - 0x3F;
#define OLED_SET_CLK_DIV ((uint8_t)0xD5)     // Enable CLK DIV / Oscillator frequency register - MUST BE FOLLOWED BY VALUE!
#define OLED_CLK_DIV_VAL ((uint8_t)0x80)     // b7 - b4 -> oscillator frequency, 0xFX fastest; b3 - b0 -> CLK DIV, 0xX0 fastest; 0x00 - 0xFF;
#define OLED_SET_PRECHARGE ((uint8_t)0xD9)   // Enable pre-charge register - MUST BE FOLLOWED BY VALUE!
#define OLED_PRECHARGE_VAL ((uint8_t)0x11)   // b7 - b4 -> Dis-charge period; b3 - b0 -> pre-charge period; 0x00 - 0xFF;
#define OLED_SET_COM_PINS ((uint8_t)0xDA)    // Enable common pads register - MUST BE FOLLOWED BY VALUE!
#define OLED_COM_PINS_VAL ((uint8_t)0x12)    // Sequential or alternative mode; 0x02 - sequential; 0x12 - alternative;
#define OLED_SET_VCOM_DESEL ((uint8_t)0xDB)  // Enable deselect level mode register - MUST BE FOLLOWED BY VALUE!
#define OLED_VCOM_DESEL_VAL ((uint8_t)0x40)  // 0x00 - 0xFF;
#define OLED_RD_MOD_WR ((uint8_t)0xE0)       // Enables Read-Modify-Write mode;
#define OLED_END_RD_MOD_WR ((uint8_t)0xEE)   // Disables Read-Modify-Write mode;
#define OLED_NOP ((uint8_t)0xE3)             // NOP command;

#ifdef __cplusplus
}
#endif

#endif // OLED_SH1106_COMMANDS_H
