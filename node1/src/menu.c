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
#include "drivers/button.h"

// Declarations so we can use them in the menu
draw_string_t main_menu_list[];
draw_string_t options_menu_list[];
draw_string_t menu_404_list[];
draw_string_t exit_list[];
menu_page_t   menu_main_page;
menu_page_t   menu_options_page;
menu_page_t   menu_404_page;
menu_page_t   menu_exit_page;

// Main menu
draw_string_t main_menu_list[] = {
  {"START",   2, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_404_page    },
  {"OPTIONS", 4, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_options_page},
  {"EXIT",    6, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_exit_page   }
};
menu_page_t menu_main_page = {main_menu_list, 3};

// Options
draw_string_t options_menu_list[] = {
  {"CALIBRATE",  2, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_404_page },
  {"BRIGHTNESS", 4, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_404_page },
  {"BACK",       6, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_main_page}
};
menu_page_t menu_options_page = {options_menu_list, 3};

// 404 - Not found
draw_string_t menu_404_list[] = {
  {"404 :(", 2, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_404_page },
  {"BACK",   6, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_main_page}
};
menu_page_t menu_404_page = {menu_404_list, 2};

// Exit
draw_string_t exit_list[] = {
  {"BYE",  2, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_exit_page},
  {"BACK", 6, OLED_WIDTH_PIXELS / 2 - 4 * 4, &menu_main_page}
};
menu_page_t menu_exit_page = {exit_list, 2};

// Current menu screen
int arrow_pos = 0;
menu_page_t *current_page = &menu_main_page;

/**
 * @brief
 * @note IMPROVEMENT: Why does arrow_pos move in increments of 2?
 *  Change to 1-2-3, etc
 *  So it doesn't point at line numbers, and instead implement this in the draw list function
 *
 * @param joystick_dir
 */
void menu_move_arrow(joystick_t joystick_dir) {
    bool    joy_up   = (joystick_dir.dir == JOYSTICK_UP && joystick_dir.dir_changed);
    bool    joy_down = (joystick_dir.dir == JOYSTICK_DOWN && joystick_dir.dir_changed);
    int8_t  dir      = joy_up - joy_down;

    uint8_t menu_page_length = current_page->list_length;

    arrow_pos -= dir;
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
void menu_draw_list() {
    // Move between pages
    if (button_right_pressed()) {
        draw_string_t *selected_option = &current_page->list[arrow_pos];
        printf("%s", selected_option->string);
        current_page = selected_option->next_page;
    }

    // Draw lines
    for (int i = 0; i < current_page->list_length; ++i) {
        draw_string_t current_option = current_page->list[i];
        oled_goto_column(current_option.column);
        oled_goto_line(current_option.line);
        oled_print_string(current_option.string);

        if (i == arrow_pos) {
            // Draw arrow
            oled_goto_column(OLED_WIDTH_PIXELS / 2 - 4 * 6);
            oled_print_char('>');
        }
    }
}
