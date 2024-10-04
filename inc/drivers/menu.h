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


typedef struct {
    char* string;
    uint8_t line;
    uint16_t column;
} draw_string;




void menu_draw_list(draw_string* draw_list, int list_length);

#endif