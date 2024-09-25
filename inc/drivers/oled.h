/**
 * @file oled.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef OLED_H
#define OLED_H

#define OLED_CMD_BASE  (volatile uint8_t *)0x1000
#define OLED_CMD_END   (volatile uint8_t *)0x11FF
#define OLED_DATA_BASE (volatile uint8_t *)0x1200
#define OLED_DATA_END  (volatile uint8_t *)0x13FF

// Command sequences
#define OLED_CMD_SET_MEMORY_ADDRESSING_MODE    0x20
#define OLED_CMD_MEMORY_ADDRESS_MODE_HORIZ     0b00
#define OLED_CMD_MEMORY_ADDRESS_MODE_VERT      0b01
#define OLED_CMD_MEMORY_ADDRESS_MODE_PAGE      0b10
#define OLED_CMD_MEMORY_ADDRESS_MODE_INVALID   0b11

#define OLED_CMD_SET_COLUMN_ADDRESS            0x21 // Followed by start and end column address, 0x00-0x7F (0 - 127)
#define OLED_CMD_SET_PAGE_ADDRESS              0x22 // Followed by start and end page address, 0x00-0x07 (0 - 7)
#define OLED_CMD_SET_DISPLAY_START_LINE        0x40 // Followed by display start line, 0x00-0x3F (0 - 63)
#define OLED_CMD_SET_CONTRAST                  0x81 // Followed by contrast value, 0x00-0xFF (0 - 255)

#define OLED_CMD_SET_SEGMENT_REMAP_NORMAL      0xA0 // Column address 0 is mapped to SEG0
#define OLED_CMD_SET_SEGMENT_REMAP_REVERSE     0xA1 // Column address 127 is mapped to SEG0

#define OLED_CMD_SET_ENTIRE_DISPLAY_ON         0xA4 // Resume to RAM content display
#define OLED_CMD_SET_ENTIRE_DISPLAY_ON_IGNORED 0xA5 // Ignore RAM content display

#define OLED_CMD_SET_NORMAL_DISPLAY            0xA6 // Normal display
#define OLED_CMD_SET_INVERSE_DISPLAY           0xA7 // Inverse display

#define OLED_CMD_SET_MULTIPLEX_RATIO           0xA8 // Followed by multiplex ratio, 0x0F-0x3F (15 - 63), reset is 0x3F (63)

#define OLED_CMD_SET_IREF_SELECTION            0xAD // ???
#define OLED_CMD_SET_IREF_SELECTION_EXTERNAL   0x00 // ???
#define OLED_CMD_SET_IREF_SELECTION_INTERNAL   0x10 // ???

#define OLED_CMD_DISPLAY_OFF                   0xAE
#define OLED_CMD_DISPLAY_ON                    0xAF

#define OLED_CMD_SET_PAGE_START_ADDRESS        0xB0 // Followed by page start address, 0x00-0x07 (0 - 7)

#include <stdint.h>
#include <avr/pgmspace.h>

void oled_init(void);
void oled_reset(void);
void oled_home(void);
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(void);
void oled_print_char(char c);
void oled_print_string(char *str);

#endif // OLED_H