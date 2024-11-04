/**
 * @file joystick.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/joystick.h"

/*****************************************************************************/
/* Local variables                                                           */
/*****************************************************************************/

static joystick_config_t joystick_calibration_config;

/*****************************************************************************/
/* Function declarations                                                     */
/*****************************************************************************/

int8_t joystick_adjust(uint8_t value, joystick_config_axis_t axis_config);
int8_t map_int8(uint8_t val, uint8_t in_min, uint8_t in_max, int8_t out_min, int8_t out_max);

/*****************************************************************************/
/* Function definitions                                                      */
/*****************************************************************************/

/**
 * @brief Initialize joystick configuration
 *
 * @param config
 * @param state
 */
void joystick_init(e_JOYSTICK_INITIALIZATION_STATE state) {
    joystick_config_t *config = &joystick_calibration_config;

    switch (state) {
    case JOYSTICK_ASSUME_IDEAL:
        config->x_config   = (joystick_config_axis_t){0, 128, 255};
        config->y_config   = (joystick_config_axis_t){0, 128, 255};
        config->calibrated = true;
        break;
    case JOYSTICK_USE_DEFAULT_CALIBRATION:
        config->x_config   = (joystick_config_axis_t){2, 158, 254};
        config->y_config   = (joystick_config_axis_t){2, 164, 254};
        config->calibrated = true;
        break;
    case JOYSTICK_CALIBRATE:
        config->calibrated = false;
        break;
    }
}

/**
 * @brief Check if joystick is calibrated
 *
 * @return true
 * @return false
 */
bool joystick_is_calibrated(void) {
    // Check if joystick is calibrated
    return joystick_calibration_config.calibrated;
}

/**
 * @brief Read adjusted x/y and raw_x/raw_y values from joystick
 *
 * @param config Calibration config to use.
 * @param prev_dir The direction of the joystick from the previous read. Used for detecting if the joystick position changed.
 * @return joystick_t
 */
joystick_t joystick_read(void) {
    joystick_config_t *config = &joystick_calibration_config;

    joystick_t position = joystick_read_raw();
    if (!config->calibrated) {
        return position;
    }

    // Adjust joystick values
    int8_t     x_adjusted = joystick_adjust(position.raw_x, config->x_config);
    int8_t     y_adjusted = joystick_adjust(position.raw_y, config->y_config);

    // X-deadzone
    if (pow(x_adjusted, 2) < pow(JOY_DEADZONE_X, 2)) {
        x_adjusted = 0;
    }
    // Y-deadzone
    if (pow(y_adjusted, 2) < pow(JOY_DEADZONE_Y, 2)) {
        y_adjusted = 0;
    }

    position.x = x_adjusted;
    position.y = y_adjusted;

    // clang-format off
    if (pow(position.x, 2) > pow(position.y, 2)) {
        if (position.x > 0) position.current_dir = JOYSTICK_RIGHT;
        else                position.current_dir = JOYSTICK_LEFT;
    } else {
        if (position.y > 0) position.current_dir = JOYSTICK_UP;
        else                position.current_dir = JOYSTICK_DOWN;
    }
    if (position.x == 0 && position.y == 0) position.current_dir = JOYSTICK_CENTER;
    // clang-format on

    // position.dir_changed = prev_dir != position.dir;

    return position;
}

/**
 * @brief Read raw x/y values from joystick
 *
 * @return joystick_t
 */
joystick_t joystick_read_raw(void) {
    uint32_t   adc_output     = adc_read() >> (2 * 8);
    // uint16_t   joystick_raw   = adc_output & 0xFFFF;
    // clang-format off
    joystick_t position = {
      .x = 0,
      .y = 0,
      .raw_x = adc_output & 0xFF,
      .raw_y = (adc_output >> 8) & 0xFF,
      .current_dir = JOYSTICK_CENTER,
      //   .previous_dir = JOYSTICK_CENTER,
    };
    // clang-format on
    return position;
}

joystick_config_t *joystick_get_config(void) { return &joystick_calibration_config; }

/**
 * @brief Adjust joystick readings according to calibration
 *
 * @param value
 * @param axis_config
 * @return int8_t
 */
int8_t joystick_adjust(uint8_t value, joystick_config_axis_t axis_config) {
    if (value < axis_config.min) {
        value = axis_config.min;
    }
    if (value > axis_config.max) {
        value = axis_config.max;
    }
    if (value < axis_config.center) {
        return map_int8(value, axis_config.min, axis_config.center, JOY_MIN, JOY_CENTER);
    } else {
        return map_int8(value, axis_config.center, axis_config.max, JOY_CENTER, JOY_MAX);
    }
}

/**
 * @brief Specialized map function for the joystick, as the input is always 0->255 (uint8_t) and the output is always -128->127 (int8_t)
 * @note This function is not a general-purpose map function, but is specialized for the joystick.
 *       THIS CAN BE OPTIMIZED, SINCE IT PERFORMS FLOATING POINT OPERATIONS AND DIVISION
 *
 * @param val
 * @param in_min
 * @param in_max
 * @param out_min
 * @param out_max
 * @return int8_t
 */
int8_t map_int8(uint8_t val, uint8_t in_min, uint8_t in_max, int8_t out_min, int8_t out_max) {
    uint8_t input     = val - in_min;
    uint8_t in_range  = in_max - in_min;
    uint8_t out_range = out_max - out_min;
    float   divisor = (float)out_range / in_range; // Using float might not be necessary, as flooring the division might be enough given it is done in the right
                                                   // order: https://stackoverflow.com/a/5732117
    int8_t mapped = out_min + divisor * input;

    return mapped;
}
