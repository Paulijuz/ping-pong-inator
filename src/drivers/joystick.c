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

/**
 * @brief Initialize joystick configuration
 *
 * @param config
 * @param state
 */
void joystick_init(joystick_config_t *config, e_JOYSTICK_INITIALIZATION_STATE state) {
    switch (state) {
    case JOYSTICK_NO_CALIBRATE:
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
 * @brief Prompt user for single joystick direction calibration
 *
 * @param axis
 * @param config
 */
void joystick_calibrate_axis(e_JOYSTICK_DIR axis, joystick_config_t *config) {
    const char *axis_names[5];
    axis_names[JOYSTICK_CENTER] = "center";
    axis_names[JOYSTICK_UP]    = "top";
    axis_names[JOYSTICK_DOWN] = "bottom";
    axis_names[JOYSTICK_RIGHT]  = "right";
    axis_names[JOYSTICK_LEFT]   = "left";

    printf("Move to %s\r\n", axis_names[axis]);
    _delay_ms(CALIBRATION_DELAY);
    printf("Calibrating...\r\n");
    uint8_t calx = 0;
    uint8_t caly = 0;
    for (int i = 0; i < 100; ++i) {
        joystick_t joystick = joystick_read_raw();
        calx += joystick.raw_x;
        caly += joystick.raw_y;
        _delay_ms(10);
    }
    calx /= 100;
    caly /= 100;

    if (axis == JOYSTICK_CENTER) {
        config->x_config.center = calx;
        config->y_config.center = caly;
    } else if (axis == JOYSTICK_UP) {
        config->y_config.max = caly;
    } else if (axis == JOYSTICK_DOWN) {
        config->y_config.min = caly;
    } else if (axis == JOYSTICK_RIGHT) {
        config->x_config.max = calx;
    } else if (axis == JOYSTICK_LEFT) {
        config->x_config.min = calx;
    }
}

/**
 * @brief Prompt user for joystick calibration
 *
 * @param config
 */
void joystick_calibrate(joystick_config_t *config) {
    printf("\r\n--- Joystick calibration ---\r\n");

    // Perform calibration
    for (int axis = 0; axis < JOYSTICK_ENUM_LENGTH; ++axis) {
        joystick_calibrate_axis(axis, config);
    }

    // Check if max and min are swapped
    if (config->x_config.max < config->x_config.min) {
        uint8_t temp         = config->x_config.max;
        config->x_config.max = config->x_config.min;
        config->x_config.min = temp;
    }
    if (config->y_config.max < config->y_config.min) {
        uint8_t temp         = config->y_config.max;
        config->y_config.max = config->y_config.min;
        config->y_config.min = temp;
    }

    printf("\r\n--- Calibration complete ---\r\n");
    printf("Joystick X: %u/%u/%u\r\n", config->x_config.min, config->x_config.center, config->x_config.max);
    printf("Joystick Y: %u/%u/%u\r\n", config->y_config.min, config->y_config.center, config->y_config.max);
    _delay_ms(CALIBRATION_DELAY);
    config->calibrated = true;
}

/**
 * @brief Read adjusted x/y and raw_x/raw_y values from joystick
 *
 * @param config Calibration config to use.
 * @param prev_dir The direction of the joystick from the previous read. Used for detecting if the joystick position changed.
 * @return joystick_t
 */
joystick_t joystick_read(joystick_config_t *config, e_JOYSTICK_DIR prev_dir) {
    joystick_t position = joystick_read_raw();

    // Comment out calibration until it is fixed.
    // position.x = joystick_adjust(position.raw_x, config->x_config);
    // position.y = joystick_adjust(position.raw_y, config->y_config);

    int8_t x = position.raw_x - 128;
    int8_t y = position.raw_y - 128;

    if (pow(x, 2) + pow(y, 2) < pow(JOY_DEADZONE,2)) return position;

    position.x = x;
    position.y = y;

    if (pow(x, 2) > pow(y, 2)) {
        if (x > 0) {
            position.dir = JOYSTICK_RIGHT;
        } else {
            position.dir = JOYSTICK_LEFT;
        }
    } else {
        if (y > 0) {
            position.dir = JOYSTICK_UP;
        } else {
            position.dir = JOYSTICK_DOWN;
        }
    }

    position.dir_changed = prev_dir != position.dir;

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
    joystick_t position = {
      .x           = 0,
      .y           = 0,
      .dir         = JOYSTICK_CENTER,
      .dir_changed = false,
      .raw_x       = adc_output & 0xFF,
      .raw_y       = (adc_output >> 8) & 0xFF,
    };
    return position;
}

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
 * @brief Map values from one range to another
 *
 * @param val
 * @param in_min
 * @param in_max
 * @param out_min
 * @param out_max
 * @return int8_t
 */
int8_t map_int8(int8_t val, int8_t in_min, int8_t in_max, int8_t out_min, int8_t out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
