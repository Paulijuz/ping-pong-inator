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
    printf("\r\n--- Program startup ---\r\n");

    // Initialize SRAM
    sram_init();
    printf("SRAM initialized\r\n");

    // Initialize ADC
    adc_init();
    printf("ADC initialized\r\n");

    // Joystick calibration and configuration
    joystick_config_t joystick_calibration_config;
    joystick_init(&joystick_calibration_config, JOYSTICK_USE_DEFAULT_CALIBRATION);
    printf("Joystick initialized\r\n");
    if (!joystick_calibration_config.calibrated) { // Calibrate joystick if not already done
        joystick_calibrate(&joystick_calibration_config);
    }

    // Initialize OLED
    oled_init();
    printf("OLED initialized\r\n");
    font_config_t font_config = FONT5_CONFIG; // Decide on font
    oled_set_font(&font_config);
    oled_clear_screen();

    // Initialize SPI
    spi_init_master();
    printf("SPI initialized\r\n");

    // Initialize CAN
    can_init();
    printf("CAN initialized\r\n");

    // Enable interrupts
    sei();

    joystick_t joystick; // = joystick_read(&joystick_calibration_config, JOYSTICK_CENTER);
    while (1) {
        // Read joystick
        joystick = joystick_read(&joystick_calibration_config, joystick.dir);

        // Navigating menu
        menu_move_arrow(joystick);
        oled_clear_screen();
        menu_draw_list();

        // Switch buffers that are drawn
        if (oled_should_flush()) {
            oled_flip_buffer();
            oled_flush_buffer();
        }

        // Transmit CAN message
        static uint8_t can_id    = 0;
        can_message_s  t_message = {
           .data   = {joystick.x, joystick.y},
           .length = 2,
           .id     = can_id++,
        };
        can_transmit(&t_message);

        // Receive CAN message
        can_message_s r_message;
        bool          can_receive_status = can_receive(&r_message);
        if (can_receive_status) {
            printf("%u: %s \r\n", r_message.id, r_message.data);
        } else {
            printf("Ingen data mottatt :(\r\n");
        }

        // What the fuck does this do
        printf("%x \r\n", mcp_read(0x30));
        // printf("%x \r\n", mcp_read_status())

        // Delay to prevent spamming
        _delay_ms(10);
    }

    return 0;
}
