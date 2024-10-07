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
#include "drivers/joystick.h"

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


void menu_switch_screen(){}




void menu_draw_list(draw_string* draw_list, int list_length) {

    for (int i = 0; i < list_length; ++i) {
        oled_goto_column(draw_list[i].column);
        oled_goto_line(draw_list[i].line);
        oled_print_string(draw_list[i].string);
    }
}
