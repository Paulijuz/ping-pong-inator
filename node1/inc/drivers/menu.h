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

#include <stdint.h>
#include "drivers/joystick.h"

#ifndef MENU_H
#define MENU_H

typedef struct draw_string_s {
    char* string;
    uint8_t line;
    uint16_t column;
    struct draw_string_s* next_screen;
} draw_string;


//menu arrow position
extern int arrow_pos;

extern draw_string main_menu_list[];
extern draw_string options_menu_list[];
extern draw_string not_impl_list[];
extern draw_string exit_list[];


extern draw_string* current_screen;

void menu_move_arrow(joystick_t joystick_dir);

void menu_draw_list(int list_length);

#endif