/**
 * @file menu.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "drivers/oled.h"
#include "drivers/menu.h"

void menu_draw_list(draw_string* draw_list, int list_length) {

    for (int i = 0; i < list_length; ++i) {
        oled_goto_column(draw_list[i].column);
        oled_goto_line(draw_list[i].line);
        oled_print_string(draw_list[i].string);
    }
}