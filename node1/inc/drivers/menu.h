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

// #include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/button.h"
#include "fsm.h"

typedef enum e_MENU_OPTION_TYPE {
    MENU_NEXT_PAGE,
    MENU_EXECUTE_FUNCTION,
} e_MENU_OPTION_TYPE;

/**
 * @brief Menu string struct
 *
 */
typedef struct menu_option_s {
    char* string;
    // uint8_t line;
    uint16_t column;

    struct menu_page_s *next_page;
    void (*execution_function)(void);
} menu_option_t;

/**
 * @brief Menu page struct
 *
 */
typedef struct menu_page_s {
    menu_option_t *list;
    uint8_t        list_length;
} menu_page_t;

void           menu_move_arrow(int dir);
void menu_draw_list(void);
menu_option_t *menu_get_selected_option(void);
void           menu_activate_selection(void);

// Specific menu functions for drawing certain pages
void menu_draw_calibration(const char *direction, float completion);

#endif
