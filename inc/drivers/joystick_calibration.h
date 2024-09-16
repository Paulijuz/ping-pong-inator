/**
 * @file joystick_calibration.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defines.h"
#include <util/delay.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "drivers/adc.h"

#define JOY_MIN    -128
#define JOY_MAX    127
#define JOY_CENTER 0

typedef struct joystick_s {
    uint8_t x_min;
    uint8_t x_off;
    uint8_t x_max;

    uint8_t y_min;
    uint8_t y_off;
    uint8_t y_max;

    bool    calibrated;
} joystick_t;

void    calibrate(joystick_t *config);
uint8_t joy_x(uint8_t adc_val, joystick_t *config);
uint8_t joy_y(uint8_t adc_val, joystick_t *config);

int16_t mapint(int16_t val, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);
