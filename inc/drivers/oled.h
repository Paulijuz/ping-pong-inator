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

#define OLED_DATA_BASE (volatile uint8_t *)0x1000
#define OLED_CMD_BASE  (volatile uint8_t *)0x1200

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

#endif // OLED_H
