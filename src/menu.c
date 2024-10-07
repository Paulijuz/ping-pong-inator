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

int arrow_pos = 2;


draw_string main_menu_list[] = {
    {"START", 2, OLED_WIDTH_PIXELS/2 -4*4, not_impl_list}, 
    {"OPTIONS", 4, OLED_WIDTH_PIXELS/2 -4*4, options_menu_list},
    {"EXIT", 6, OLED_WIDTH_PIXELS/2 -4*4, exit_list}
};

draw_string options_menu_list[] = {
    {"CALIBRATE", 2, OLED_WIDTH_PIXELS/2 -4*4, not_impl_list},
    {"BRIGHTNESS", 4, OLED_WIDTH_PIXELS/2 -4*4, not_impl_list},
    {"BACK", 6, OLED_WIDTH_PIXELS/2 -4*4, main_menu_list}
};

draw_string not_impl_list[] = {
    {"NOT YET", 2, OLED_WIDTH_PIXELS/2 -4*4, not_impl_list },
    {"IMPLEMENTED", 4, OLED_WIDTH_PIXELS/2 -4*4, not_impl_list},
    {"BACK :(", 6, OLED_WIDTH_PIXELS/2 -4*4, main_menu_list}
};

draw_string exit_list[] = {
    {"", 2, OLED_WIDTH_PIXELS/2 -4*4, exit_list},
    {"", 4, OLED_WIDTH_PIXELS/2 -4*4, exit_list},
    {"", 6, OLED_WIDTH_PIXELS/2 -4*4, exit_list}
};

draw_string* current_screen = main_menu_list;

void menu_move_arrow(joystick_t joystick_dir) {
     if (joystick_dir.dir == JOYSTICK_UP && joystick_dir.dir_changed) {
            arrow_pos -= 2;
            if (arrow_pos < 2) {
                arrow_pos = 6;
            }

        } else if(joystick_dir.dir == JOYSTICK_DOWN && joystick_dir.dir_changed) {
            arrow_pos += 2;
             if (arrow_pos > 6) {
                arrow_pos = 2;
            }
            
        }
}




void menu_draw_list(int list_length) {


    if (button_right_pressed()) {
        printf("%s", current_screen[arrow_pos/2-1].string);
        current_screen = current_screen[arrow_pos/2 -1].next_screen;
    }


    for (int i = 0; i < list_length; ++i) {
        oled_goto_column(current_screen[i].column);
        oled_goto_line(current_screen[i].line);
        oled_print_string(current_screen[i].string);
    }
}
