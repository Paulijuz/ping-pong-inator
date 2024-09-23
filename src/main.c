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
#include "drivers/joystick.h"

// Cutoff frequency of filter: 795 Hz
// Slope: 20 dB per decade

int main(void) {
    MCUCR |= 1 << SRE; // MCU Control Register bit SRE ??

    // Program startup
    printf("\r\n--- Program startup ---\r\n");

    // Initialize UART
    uart_init(9600);
    printf("UART initialized\r\n");

    // Initialize ADC
    adc_init();
    printf("ADC initialized\r\n");

    // Joystick calibration and configuration
    // If default values are desired, they are here:
    joystick_config_t joystick_calibration_config = {
      {2, 158, 254}, // X-calibration
      {2, 164, 254}, // Y-calibration
      true, // Already calibrated
    };

    while (1) {
        // Calibrate joystick if not already done
        if (!joystick_calibration_config.calibrated) {
            joystick_calibrate(&joystick_calibration_config);
        }

        // Read and print joystick position (both calibrated and raw values)
        joystick_t joy = joystick_read(&joystick_calibration_config);
        printf("Joystick: %03d, %03d | %03d, %03d\r\n", joy.x, joy.y, joy.raw_x, joy.raw_y);
    }

    return 0;
}
