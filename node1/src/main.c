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
#include "drivers/button.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/sliders.h"
#include "drivers/menu.h"
#include "drivers/spi.h"
#include "drivers/mcp.h"
#include "drivers/can.h"
#include "fonts.h"
#include "fsm.h"
#include "logger.h"

// Something something assignment with a filter
// Cutoff frequency of filter: 795 Hz
//            Slope of filter: 20 dB per decade

/**
 * @brief Main function
 *
 * @return int
 */
int main(void) {
    cli(); // Prevent interrupts during initialization

    // Initialize UART
    uart_init(9600);
    uart_enable_printf();

    // Program startup
    printf("\r\n");
    log_info("Program startup");

    // Initialize SRAM
    sram_init();
    // log_info("SRAM initialized");

    // Initialize ADC
    adc_init();
    // log_info("ADC initialized");

    // Initialize OLED
    oled_init();
    // log_info("OLED initialized");
    font_config_t font_config = FONT5_CONFIG; // Decide on font
    oled_set_font(&font_config);
    // log_info("Font set");
    oled_clear_screen();

    // Initialize SPI
    spi_init_master();
    // log_info("SPI initialized");

    // Initialize CAN
    can_init();
    // log_info("CAN initialized");

    // Joystick calibration and configuration
    joystick_init(JOYSTICK_USE_DEFAULT_CALIBRATION);
    // log_info("Joystick initialized");

    // Reenable interrupts
    sei();

    // Continuous operation
    while (1) {
        // OLED stuff
        oled_clear_screen();

        // FSM
        fsm();

        // Switch buffers that are drawn
        if (oled_should_flush()) {
            oled_flip_buffer();
            oled_flush_buffer();
        }

        // Delay to prevent spamming
        _delay_ms(10);
    }

    return 0;
}
