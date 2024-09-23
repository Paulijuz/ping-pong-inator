/**
 * @file joystick.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief Joystick driver with calibration support
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "defines.h" // Required for F_CPU, used in util/delay.h
#include <util/delay.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "drivers/adc.h"

/**
 * @brief Joystick calibration values
 *
 */
#define JOY_MIN    -128
#define JOY_MAX    127
#define JOY_CENTER 0

#define CALIBRATION_DELAY 1500 // ms, delay between calibration steps (for human interaction)

/**
 * @brief Struct for joystick calibration configuration
 *
 */
typedef struct joystick_config_axis_s {
    uint8_t min;
    uint8_t center;
    uint8_t max;
} joystick_config_axis_t;

/**
 * @brief Struct for joystick configuration
 *
 */
typedef struct joystick_config_s {
    joystick_config_axis_t x_config;
    joystick_config_axis_t y_config;
    bool                   calibrated;
} joystick_config_t;

/**
 * @brief Struct for holding raw and calibrated joystick data
 *
 */
typedef struct joystick_s {
    int8_t  x;
    int8_t  y;

    uint8_t raw_x;
    uint8_t raw_y;
} joystick_t;

void       joystick_calibrate(joystick_config_t *config);
joystick_t joystick_read(joystick_config_t *config);
int8_t     joystick_adjust(uint8_t value, joystick_config_axis_t axis_config);

int8_t     map_int(int8_t val, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max);

#endif // JOYSTICK_H
