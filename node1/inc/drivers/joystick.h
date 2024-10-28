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
#include "drivers/adc.h"

#include <math.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Joystick calibration values
 *
 */
#define JOY_MIN           -128
#define JOY_MAX           127
#define JOY_CENTER        0

/**
 * @brief Deadzone for joystick
 *
 */
#define JOY_DEADZONE_X 0
#define JOY_DEADZONE_Y 50 // Larger because Y is used for menu

/**
 * @brief Delay for calibration
 *
 */
#define CALIBRATION_DELAY 1500 // ms, delay between calibration steps (for human interaction)

/**
 * @brief Enum for joystick initialization state
 *
 */
typedef enum JOYSTICK_INITIALIZATION_STATE {
    JOYSTICK_NO_CALIBRATE,
    JOYSTICK_USE_DEFAULT_CALIBRATION,
    JOYSTICK_CALIBRATE
} e_JOYSTICK_INITIALIZATION_STATE;

// clang-format off
/**
 * @brief Enum for joystick direction
 *
 */
typedef enum JOYSTICK_DIR {
    JOYSTICK_CENTER,
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_RIGHT,
    JOYSTICK_LEFT,

    JOYSTICK_ENUM_LENGTH,
} e_JOYSTICK_DIR;
// clang-format on

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

    e_JOYSTICK_DIR dir;
    bool dir_changed;

    uint8_t raw_x;
    uint8_t raw_y;
} joystick_t;

void       joystick_init(joystick_config_t *config, e_JOYSTICK_INITIALIZATION_STATE state);
void       joystick_calibrate(joystick_config_t *config);
joystick_t joystick_read(joystick_config_t *config, e_JOYSTICK_DIR prev_dir);
joystick_t joystick_read_raw(void);
int8_t     joystick_adjust(uint8_t value, joystick_config_axis_t axis_config);

int8_t     map_int8(uint8_t val, uint8_t in_min, uint8_t in_max, int8_t out_min, int8_t out_max);

#endif // JOYSTICK_H
