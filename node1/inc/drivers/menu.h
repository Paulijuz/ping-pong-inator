/**
 * @file menu.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include "drivers/joystick.h"

/**
 * @brief Menu string struct
 *
 */
typedef struct draw_string_s {
    char* string;
    uint8_t line;
    uint16_t column;
    struct menu_page_s *next_page;
} draw_string_t;

/**
 * @brief Menu page struct
 *
 */
typedef struct menu_page_s {
    draw_string_t *list;
    uint8_t        list_length;
} menu_page_t;

// Menu arrow position
extern int arrow_pos;

// Current menu screen
extern menu_page_t *current_page;

void menu_move_arrow(joystick_t joystick_dir);
void menu_draw_list(void);

#endif
