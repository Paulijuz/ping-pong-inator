/**
 * @file joystick_calibration.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/joystick_calibration.h"

#define CALIBRATION_DELAY 4000

void calibrate(joystick_t *config) {
    printf("\r\n--- Initiating joystick calibration ---\r\n");
    uint16_t calx = 0;
    uint16_t caly = 0;
    // char    *progress_bar = "##########";
    uint16_t output;

    // Center calibration
    printf("Move joystick to center position\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating center position...\r\n");
    calx = 0;
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        output = adc_read();
        calx += output & 0xFF;
        caly += (output >> 8) & 0xFF;
        // printf("Calibrating | %.*s | %03d%%", 10, progress_bar, 0);
        _delay_ms(10);
    }
    config->x_off = calx / 100;
    config->y_off = caly / 100;
    printf("Center calibration complete: X: %03u, Y: %03u\r\n", config->x_off, config->y_off);

    // Top calibration
    printf("Move joystick to top position\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating top position...\r\n");
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        output = adc_read();
        caly += (output >> 8) & 0xFF;
        // printf("Calibrating | %.*s | %03d%%", 10, progress_bar, 0);
        _delay_ms(10);
    }
    config->y_max = caly / 100;
    printf("Top calibration complete: Y: %03u\r\n", config->y_max);

    // Bottom calibration
    printf("Move joystick to bottom position\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating bottom position...\r\n");
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        output = adc_read();
        caly += (output >> 8) & 0xFF;
        // printf("Calibrating | %.*s | %03d%%", 10, progress_bar, 0);
        _delay_ms(10);
    }
    config->y_min = caly / 100;
    printf("Bottom calibration complete: Y: %03u\r\n", config->y_min);

    // Right calibration
    printf("Move joystick to right position\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating right position...\r\n");
    calx = 0;
    for (int i = 0; i < 100; ++i) {
        output = adc_read();
        calx += output & 0xFF;
        // printf("Calibrating | %.*s | %03d%%", 10, progress_bar, 0);
        _delay_ms(10);
    }
    config->x_max = caly / 100;
    printf("Right calibration complete: X: %03u\r\n", config->x_max);

    // Left calibration
    printf("Move joystick to left position\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating left position...\r\n");
    calx = 0;
    for (int i = 0; i < 100; ++i) {
        output = adc_read();
        calx += output & 0xFF;
        // printf("Calibrating | %.*s | %03d%%", 10, progress_bar, 0);
        _delay_ms(10);
    }
    config->x_min = caly / 100;
    printf("Left calibration complete: X: %03u\r\n", config->x_min);

    // Check if max and min are swapped
    if (config->x_max < config->x_min) {
        uint8_t temp  = config->x_max;
        config->x_max = config->x_min;
        config->x_min = temp;
    }
    if (config->y_max < config->y_min) {
        uint8_t temp  = config->y_max;
        config->y_max = config->y_min;
        config->y_min = temp;
    }

    printf("\r\n--- Joystick calibration complete ---\r\n");
    printf("Joystick X: %u/%u/%u\r\n", config->x_min, config->x_off, config->x_max);
    printf("Joystick Y: %u/%u/%u\r\n", config->y_min, config->y_off, config->y_max);
    _delay_ms(CALIBRATION_DELAY);
    config->calibrated = true;
}

/**
 * @brief Joystick X axis
 *
 * @param adc_val
 * @param config
 * @return uint8_t
 */
uint8_t joy_x(uint8_t adc_val, joystick_t *config) {
    // int16_t xval = adc_val + (config->x_off - 128);
    // int16_t xval = min(max(adc_val, config->x_min), config->x_max);
    int16_t xval = adc_val;
    if (xval < config->x_min) {
        xval = config->x_min;
    }

    if (xval < config->x_off) {
        return mapint(xval, config->x_min, config->x_off, JOY_MIN, JOY_CENTER);
    } else {
        return mapint(xval, config->x_off, config->x_max, JOY_CENTER, JOY_MAX);
    }
}

/**
 * @brief Joystick Y axis
 *
 * @param adc_val
 * @param config
 * @return uint8_t
 */
uint8_t joy_y(uint8_t adc_val, joystick_t *config) {
    // int16_t xval = adc_val + (config->x_off - 128);
    // int16_t yval = min(max(adc_val, config->y_min), config->y_max);
    int16_t yval = adc_val;
    if (yval < config->y_off) {
        return mapint(yval, config->y_min, config->y_off, JOY_MIN, JOY_CENTER);
    } else {
        return mapint(yval, config->y_off, config->y_max, JOY_CENTER, JOY_MAX);
    }
}

int16_t mapint(int16_t val, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
