/**
 * @file joystick.h
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

typedef struct joystick_config_axis_s {
    uint8_t min;
    uint8_t center;
    uint8_t max;
} joystick_config_axis_t;

typedef struct joystick_config_s {
    joystick_config_axis_t x_config;
    joystick_config_axis_t y_config;
    bool                   calibrated;
} joystick_config_t;

typedef struct joystick_s {
    uint8_t x;
    uint8_t y;

    uint8_t raw_x;
    uint8_t raw_y;
} joystick_t;

void       joystick_calibrate(joystick_config_t *config);
joystick_t joystick_read(joystick_config_t *config);
int8_t     joystick_adjust(uint8_t value, joystick_config_axis_t axis_config);

int8_t     mapint(int8_t val, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max);
