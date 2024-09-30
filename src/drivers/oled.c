/**
 * @file oled.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/oled.h"
#include "fonts.h"

static font_config_t font_config = {FONT_8, 8};

/**
 * @brief Write command to OLED
 *
 * @param c
 */
void oled_cmd_write_char(char c) {
    // Write to address base
    *OLED_CMD_BASE = c;
}

/**
 * @brief Initialize OLED (from OLED datasheet, see lab support data)
 *
 */
void oled_init(void) {
    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_OFF);           // display off
    oled_cmd_write_char(OLED_CMD_SET_SEGMENT_REMAP_REVERSE); // segment remap

    oled_cmd_write_char(OLED_CMD_SET_COM_PINS_HW_CONF);      // common pads hardware: alternative
    oled_cmd_write_char(OLED_CMD_SET_COM_PINS_HW_CONF_ALT_DIS);

    oled_cmd_write_char(OLED_CMD_SET_COM_OUTPUT_SCAN_DIR_REV); // common output scan direction:com63~com0 = reversed

    oled_cmd_write_char(OLED_CMD_SET_MULTIPLEX_RATIO);         // multiplex ration mode:63
    oled_cmd_write_char(0x3f);

    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE); // display divide ratio/osc. freq. mode
    oled_cmd_write_char(0x80);

    oled_cmd_write_char(OLED_CMD_SET_CONTRAST); // contrast control
    oled_cmd_write_char(0x50);

    oled_cmd_write_char(OLED_CMD_SET_PRECHARGE_PERIOD); // set pre-charge period
    oled_cmd_write_char(0x21);

    oled_cmd_write_char(OLED_CMD_SET_MEMORY_ADDRESSING_MODE); // Set Memory Addressing Mode
    oled_cmd_write_char(OLED_CMD_MEMORY_ADDRESS_MODE_HORIZ);

    oled_cmd_write_char(OLED_CMD_SET_VCOMH_DESELECT_LEVEL); // VCOM deselect level mode
    oled_cmd_write_char(OLED_CMD_VCMOH_DESELECT_LEVEL_2);

    oled_cmd_write_char(OLED_CMD_SET_IREF_SELECTION); // master configuration
    oled_cmd_write_char(OLED_CMD_SET_IREF_SELECTION_EXTERNAL);

    oled_cmd_write_char(OLED_CMD_SET_ENTIRE_DISPLAY_ON); // out follows RAM content
    oled_cmd_write_char(OLED_CMD_SET_NORMAL_DISPLAY);    // set normal display
    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_ON);        // display on
}

void oled_reset(void) {}
void oled_home(void) {}

/**
 * @brief Go to line (page) on OLED
 *
 * @param line
 */
void oled_goto_line(uint8_t line) {
    // Move to page
    oled_cmd_write_char(0xb0 + line);
}

void oled_goto_column(uint8_t column);

/**
 * @brief Clear line on OLED
 *
 * @param line
 */
void oled_clear_line(uint8_t line) {
    oled_goto_line(line);
    for (uint8_t i = 0; i < 128; ++i) {
        *OLED_DATA_BASE = 0x00;
    }
}

void oled_pos(void);

/**
 * @brief Set font for OLED
 *
 * @param font
 */
void oled_set_font(font_config_t *font) {
    // Set local font configuration
    font_config = *font;
}

void oled_print_char(char c) {
    if (c < 32 || c > 126) {
        c = 126;
    }

    // Special case for space
    if (c == 32) {
        for (int i = 0; i < font_config.font_width; ++i) {
            *OLED_DATA_BASE = 0x00;
        }
        return;
    }

    // Read font from program memory
    // FROM: https://www.nongnu.org/avr-libc/user-manual/pgmspace.html
    // memcpy_P((void *)OLED_DATA_BASE, font4[c - 32], 4); // This can be unreliable for some reason??
    for (int i = 0; i < font_config.font_width; ++i) {
        switch (font_config.font) {
        case FONT_8:
            *OLED_DATA_BASE = pgm_read_byte(&(font8[c - 32][i]));
            break;
        case FONT_5:
            *OLED_DATA_BASE = pgm_read_byte(&(font5[c - 32][i]));
            break;
        case FONT_4:
            *OLED_DATA_BASE = pgm_read_byte(&(font4[c - 32][i]));
            break;
        }
    }
}

/**
 * @brief Print string to OLED
 *
 * @param str
 */
void oled_print_string(char *str) {
    while (*str) {
        oled_print_char(*str++);
    }
}
