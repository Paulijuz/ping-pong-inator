/**
 * @file main.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "defines.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

// #include "sram_test.h"

#include "drivers/uart.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/menu.h"
#include "fonts.h"

// Cutoff frequency of filter: 795 Hz
// Slope: 20 dB per decade

int main(void) {
    cli();

    draw_string main_menu_list[] = {
    {"START", 2, OLED_WIDTH_PIXELS/2 -4*4},
    {"OPTIONS", 4, OLED_WIDTH_PIXELS/2 -4*4},
    {"EXIT", 6, OLED_WIDTH_PIXELS/2 -4*4}
    };

    // Initialize SRAM
    sram_init();

    // Initialize UART
    uart_init(9600);

    // Program startup
    printf("\r\n--- Program startup ---\r\n");

    // Initialize ADC
    adc_init();
    printf("ADC initialized\r\n");

    // Joystick calibration and configuration
    // If default values are desired, they are here:
    joystick_config_t joystick_calibration_config;
    joystick_init(&joystick_calibration_config, JOYSTICK_USE_DEFAULT_CALIBRATION);
    printf("Joystick initialized\r\n");
    if (!joystick_calibration_config.calibrated) { // Calibrate joystick if not already done
        joystick_calibrate(&joystick_calibration_config);
    }

    // Initialize OLED
    oled_init();
    printf("OLED initialized\r\n");

    // Decide on font
    font_config_t font_config = FONT5_CONFIG;
    oled_set_font(&font_config);
    oled_clear_screen();
    sei();

    joystick_t joystick_dir = joystick_read(&joystick_calibration_config, JOYSTICK_CENTER);
    int arrow_pos = 0;

    while (1) {
        // Read and print joystick position (both calibrated and raw values)
        // joystick_t joy = joystick_read(&joystick_calibration_config);
        // printf("Joystick: %03d, %03d | %03d, %03d\r\n", joy.x, joy.y, joy.raw_x, joy.raw_y);

        // Chip-select test for OLED
        // volatile uint8_t *base = (uint8_t *)0x1000;
        // volatile uint8_t *end  = (uint8_t *)0x1600;
        // for (volatile uint8_t *addr = base; addr < end; addr += 4) {
        //     *addr = 0xFF;
        //     printf("Wrote 0xFF to address %p\r\n", addr);
        // }

        // Data test for OLED
        // volatile uint8_t *base = (uint8_t *)0x1200;
        // *base                  = 0xFF;

        // Char test for OLED
        // char c = uart_receive_char();
        // if (c == 13 ) {
        //     oled_clear_line(oled_get_line());
        // } else {
        //     oled_print_char(c);
        // }
        // oled_flip_buffer();
        // oled_print_string("Hello, world!  ");
        // _delay_ms(15);



        //List test for Menu


        joystick_dir = joystick_read(&joystick_calibration_config, joystick_dir.dir);

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
        if (joystick_dir.dir_changed) {
        printf("%d \r \n",joystick_dir.dir);
        }
        oled_clear_screen();

        menu_draw_list(main_menu_list, 3);
        oled_goto_column(OLED_WIDTH_PIXELS/2 -4*6);
        oled_goto_line(arrow_pos);
        oled_print_char('>');
        oled_flip_buffer();

        if (oled_should_flush()) {
            oled_flush_buffer();
        }

    }

    return 0;
}
