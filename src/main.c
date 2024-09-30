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

// #include "sram_test.h"

#include "drivers/uart.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "fonts.h"

// Cutoff frequency of filter: 795 Hz
// Slope: 20 dB per decade

int main(void) {

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
        char c = uart_receive_char();
        oled_print_char(c);
        // oled_print_string("Hello, world!  ");
        // _delay_ms(15);
    }

    return 0;
}
