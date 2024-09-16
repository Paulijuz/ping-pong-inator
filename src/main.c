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

#include "sram_test.h"

#include "drivers/uart.h"
#include "drivers/adc.h"
#include "drivers/joystick_calibration.h"

// Cutoff frequency of filter: 795 Hz
// Slope: 20 dB per decade

int main(void) {
    // DDRB = 0xff; // Set Data Direction Register B ??
    // int status = 1;
    // PORTB      = status;

    MCUCR |= 1 << SRE; // MCU Control Register bit SRE ??
    // DDRD = 0xFF;

    uart_init(9600);

    printf("Startup\r\n");

    // Buttons
    // DDRB |= (1 << 0) | (1 << 1);

    // Sliders

    // while (1) {
    //     SRAM_test();
    //     _delay_ms(10000);
    // }

    // Joystick / ADC
    adc_init();
    printf("ADC initialized\r\n");
    // _delay_ms(1000);
    joystick_t calibration_config;
    calibration_config.calibrated = false;

    while (1) {
        // printf("Hello, World!\r\n");
        if (!calibration_config.calibrated) {
            calibrate(&calibration_config);
        }
        // counter = !counter;
        // if (counter) {
        //     PORTD |= (1 << 5);
        // } else {
        //     PORTD &= ~(1 << 5);
        // }
        // _delay_ms(1);

        // printf("%c", data);
        // printf("Hello, World! Iteration: %d\r\n", counter++);
        // _delay_ms(100);

        // CS test
        // adc_cs();
        // _delay_ms(10);
        volatile uint32_t adc_output = adc_read();

        volatile uint16_t joystick = adc_output;
        volatile uint16_t slider = adc_output >> 2*8;

        volatile uint8_t joystick_x = joystick & 0xFF;
        volatile uint8_t joystick_y = (joystick >> 8) & 0xFF;
        // printf("Hello, World %d!\r\n", 14);
        // if (printf("Joystick: %d %d\r\n", 14, 41) < 0) {
        //     printf("FAIL\r\n");
        // }
        // printf("Joystick: %d %d\r\n", joystick_x, joystick_y);
        // printf("Joystick: %u", joystick_x);
        // printf(" %u\r\n", joystick_y);
        // printf("Joystick: %u, %u", joystick_x, joystick_y);
        // fflush(stdout);

        // volatile uint8_t slider_x = slider & 0xFF;
        // volatile uint8_t slider_y = (slider >> 8) & 0xFF;
        // printf(", slider: %3u, %3u\r\n", slider_x, slider_y);
        // printf("Slider: %s\r\n", "Hei!");

        // if (PORTB & 1) {
        //     printf("Button 1 pressed\r\n");
        // }
        // if (PORTB & 2) {
        //     printf("Button 2 pressed\r\n");
        // }

        int8_t joyx = joy_x(joystick_x, &calibration_config);
        int8_t joyy = joy_y(joystick_y, &calibration_config);
        printf("Joystick: %03d, %03d | %03d, %03d\r\n", joyx, joyy, joystick_x, joystick_y);

        // Write signal
        // Wait
        // Read
    }

    // while (1) {
    //     for (int i = 0x1400; i<=0x1744; i++) {
    //         *(int *)i = 0xFF;
    //         _delay_ms(1);
    //     }
    //     for (int i = 0x1800; i<=0x1FFF; i++) {
    //         *(int *)i = 0xFF;
    //         _delay_ms(1);
    //     }
    // }

    // while (1) {
    //     char data = uart_receive_char();
    //     if (data == 'e') {
    //         PORTB  = status;
    //         status = !status;
    //     }
    //     printf("%c", data);
    // }

    return 0;
}
