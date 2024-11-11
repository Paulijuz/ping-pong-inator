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

#include "drivers/menu.h"

/*****************************************************************************/
/* Local variables                                                           */
/*****************************************************************************/

// Declarations so we can use them in the menu
menu_option_t main_menu_list[];
menu_option_t options_menu_list[];
menu_option_t menu_404_list[];
menu_option_t exit_list[];
menu_page_t   menu_main_page;
menu_page_t   menu_options_page;
menu_page_t   menu_404_page;
menu_page_t   menu_exit_page;

#define MENU_LEFT  OLED_WIDTH_PIXELS / 2 - 4 * 4
#define ARROW_LEFT OLED_WIDTH_PIXELS / 2 - 4 * 6

// Main menu
menu_option_t main_menu_list[] = {
  {"START",   MENU_LEFT, NULL,               fsm_goto_game},
  {"OPTIONS", MENU_LEFT, &menu_options_page, NULL         },
  {"EXIT",    MENU_LEFT, &menu_exit_page,    NULL         },
};
menu_page_t menu_main_page = {main_menu_list, 3};

// Options
menu_option_t options_menu_list[] = {
  {"CALIBRATE",  MENU_LEFT, NULL,            fsm_goto_calibration},
  {"BRIGHTNESS", MENU_LEFT, &menu_404_page,  NULL                },
  {"BACK",       MENU_LEFT, &menu_main_page, NULL                }
};
menu_page_t menu_options_page = {options_menu_list, 3};

// 404 - Not found
menu_option_t menu_404_list[] = {
  {"404 :(", MENU_LEFT, &menu_404_page,  NULL},
  {"BACK",   MENU_LEFT, &menu_main_page, NULL}
};
menu_page_t menu_404_page = {menu_404_list, 2};

// Exit
menu_option_t exit_list[] = {
  {"BYE",  MENU_LEFT, &menu_exit_page, NULL},
  {"BACK", MENU_LEFT, &menu_main_page, NULL}
};
menu_page_t menu_exit_page = {exit_list, 2};

// Current menu screen
static int          arrow_pos    = 0;
static menu_page_t *current_page = &menu_main_page;

/*****************************************************************************/
/* Local function declarations                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Function definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * @note IMPROVEMENT: Why does arrow_pos move in increments of 2?
 *  Change to 1-2-3, etc
 *  So it doesn't point at line numbers, and instead implement this in the draw list function
 *
 * @param joystick_dir
 */
void menu_move_arrow(int dir) {
    uint8_t menu_page_length = current_page->list_length;

    arrow_pos += dir;
    if (arrow_pos < 0) {
        arrow_pos = menu_page_length - 1;
    } else if (arrow_pos >= menu_page_length) {
        arrow_pos = 0;
    }
}

/**
 * @brief
 * @note IMPROVEMENT: Checking for button press should not happen in draw function
 *
 * @param list_length
 */
void menu_draw_list(void) {
    // Draw lines
    for (int i = 0; i < current_page->list_length; ++i) {
        menu_option_t current_option = current_page->list[i];
        oled_goto_column(current_option.column);
        oled_goto_line(i * 2 + 2);
        oled_print_string(current_option.string);

        if (i == arrow_pos) {
            // Draw arrow
            oled_goto_column(ARROW_LEFT);
            oled_print_char('>');
        }
    }
}

void menu_draw_calibration(const char *direction, float completion) {
    oled_goto_line(2);
    oled_goto_column(16);
    oled_print_string("CALIBRATION");

    oled_goto_line(4);
    oled_goto_column(16);
    oled_print_string("Move to ");
    oled_print_string(direction);

    oled_goto_line(6);
    oled_goto_column(16);
    oled_print_string("Progress: ");
    int    completion_int    = (int)(completion * 100);
    size_t completion_length = (size_t)((ceil(log10(completion_int)) + 1) * sizeof(char));
    char   completion_string[completion_length];
    sprintf(completion_string, "%d", completion_int);
    oled_print_string(completion_string);
    oled_print_string("%");
}

void menu_draw_game(const uint8_t lives, const uint8_t max_lives) {
    oled_goto_line(2);
    oled_goto_column(16);
    oled_print_string("GAME");

    oled_goto_line(4);
    oled_goto_column(16);
    oled_print_string("Lives: ");
    oled_print_char((char)lives);
    oled_print_string(" / ");
    oled_print_char((char)max_lives);
}

void menu_draw_game_over(void) {
    oled_goto_line(2);
    oled_goto_column(16);
    oled_print_string("GAME OVER");

    oled_goto_line(4);
    oled_goto_column(16);
    oled_print_string("Press right button");
}

/**
 * @brief
 *
 * @return menu_option_t*
 */
menu_option_t *menu_get_selected_option() {
    // Return current pointed to option
    return &current_page->list[arrow_pos];
}

/**
 * @brief
 *
 * @param page
 */
void menu_activate_selection() {
    menu_option_t *option = menu_get_selected_option();
    if (option->next_page == NULL && option->execution_function == NULL) {
        return;
    }

    if (option->next_page == NULL) {
        // Execute function
        option->execution_function();
    } else {
        // Change page
        current_page = option->next_page;
        arrow_pos    = 0;
    }
}
