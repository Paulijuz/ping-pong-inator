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

// clang-format off
typedef enum JOYSTICK_DIR {
    JOYSTICK_CENTER,
    JOYSTICK_TOP,
    JOYSTICK_BOTTOM,
    JOYSTICK_RIGHT,
    JOYSTICK_LEFT
} e_JOYSTICK_DIR;
// clang-format on

#define CALIBRATION_DELAY 1500

void calibrate_axis(e_JOYSTICK_DIR axis, joystick_t *config) {
    const char *axis_names[5];
    axis_names[JOYSTICK_CENTER] = "center";
    axis_names[JOYSTICK_TOP]    = "top";
    axis_names[JOYSTICK_BOTTOM] = "bottom";
    axis_names[JOYSTICK_RIGHT]  = "right";
    axis_names[JOYSTICK_LEFT]   = "left";

    printf("Move to %s", axis_names[axis]);
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    uint8_t calx = 0;
    uint8_t caly = 0;
    for (int i = 0; i < 100; ++i) {
        uint16_t joystick = adc_read();
        calx += joystick & 0xFF;
        caly += (joystick >> 8) & 0xFF;
        _delay_ms(10);
    }
    calx /= 100;
    caly /= 100;

    if (axis == JOYSTICK_CENTER) {
        config->x_off = calx;
        config->y_off = caly;
    } else if (axis == JOYSTICK_TOP) {
        config->y_max = caly;
    } else if (axis == JOYSTICK_BOTTOM) {
        config->y_min = caly;
    } else if (axis == JOYSTICK_RIGHT) {
        config->x_max = calx;
    } else if (axis == JOYSTICK_LEFT) {
        config->x_min = calx;
    }
}

void calibrate(joystick_t *config) {
    printf("\r\n--- Joystick calibration ---\r\n");
    uint16_t calx = 0;
    uint16_t caly = 0;
    uint16_t joystick;

    // Center calibration
    printf("Move to center\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    calx = 0;
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        joystick = adc_read();
        calx += joystick & 0xFF;
        caly += (joystick >> 8) & 0xFF;
        _delay_ms(10);
    }
    config->x_off = calx / 100;
    config->y_off = caly / 100;

    // Top calibration
    printf("Move to top\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        joystick = adc_read();
        caly += (joystick >> 8) & 0xFF;
        _delay_ms(10);
    }
    config->y_max = caly / 100;

    // Bottom calibration
    printf("Move to bottom\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    caly = 0;
    for (int i = 0; i < 100; ++i) {
        joystick = adc_read();
        caly += (joystick >> 8) & 0xFF;
        _delay_ms(10);
    }
    config->y_min = caly / 100;

    // Right calibration
    printf("Move to right\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    calx = 0;
    for (int i = 0; i < 100; ++i) {
        joystick = adc_read();
        calx += joystick & 0xFF;
        _delay_ms(10);
    }
    config->x_max = calx / 100;

    // Left calibration
    printf("Move to left\r\n");
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    calx = 0;
    for (int i = 0; i < 100; ++i) {
        joystick = adc_read();
        calx += joystick & 0xFF;
        _delay_ms(10);
    }
    config->x_min = calx / 100;

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

    printf("\r\n--- Calibration complete ---\r\n");
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
int8_t joy_x(uint8_t adc_val, joystick_t *config) {
    // int8_t xval = adc_val + (config->x_off - 128);
    // int8_t xval = min(max(adc_val, config->x_min), config->x_max);
    int8_t xval = adc_val;
    if (xval < config->x_min) {
        xval = config->x_min;
    }
    if (xval > config->x_max) {
        xval = config->x_max;
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
int8_t joy_y(uint8_t adc_val, joystick_t *config) {
    // int8_t xval = adc_val + (config->x_off - 128);
    // int8_t yval = min(max(adc_val, config->y_min), config->y_max);
    int8_t yval = adc_val;
    if (yval < config->y_min) {
        yval = config->y_min;
    }
    if (yval > config->y_max) {
        yval = config->y_max;
    }
    if (yval < config->y_off) {
        return mapint(yval, config->y_min, config->y_off, JOY_MIN, JOY_CENTER);
    } else {
        return mapint(yval, config->y_off, config->y_max, JOY_CENTER, JOY_MAX);
    }
}

int8_t mapint(int8_t val, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
