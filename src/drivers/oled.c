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
 * @brief Initialize OLED
 *
 */
void oled_init(void) {
    oled_cmd_write_char(0xae); // display off
    oled_cmd_write_char(0xa1); // segment remap
    oled_cmd_write_char(0xda); // common pads hardware: alternative
    oled_cmd_write_char(0x12);
    oled_cmd_write_char(0xc8); // common output scan direction:com63~com0
    oled_cmd_write_char(0xa8); // multiplex ration mode:63
    oled_cmd_write_char(0x3f);
    oled_cmd_write_char(0xd5); // display divide ratio/osc. freq. mode
    oled_cmd_write_char(0x80);
    oled_cmd_write_char(0x81); // contrast control
    oled_cmd_write_char(0x50);
    oled_cmd_write_char(0xd9); // set pre-charge period
    oled_cmd_write_char(0x21);
    oled_cmd_write_char(0x20); // Set Memory Addressing Mode
    oled_cmd_write_char(0x02);
    oled_cmd_write_char(0xdb); // VCOM deselect level mode
    oled_cmd_write_char(0x30);
    oled_cmd_write_char(0xad); // master configuration
    oled_cmd_write_char(0x00);
    oled_cmd_write_char(0xa4); // out follows RAM content
    oled_cmd_write_char(0xa6); // set normal display
    oled_cmd_write_char(0xaf); // display on
}

void oled_reset(void);
void oled_home(void);
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(void);
void oled_print_char(char c);
