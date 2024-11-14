/**
 * @file sliders.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/sliders.h"

/**
 * @brief Initialize slider configuration
 *
 * @param config
 * @param state
 */
void slider_init(slider_config_t *config, e_SLIDER_INITIALIZATION_STATE state) {
    switch (state) {
    case SLIDER_NO_CALIBRATE:
        config->calibrated = true;
        break;
    case SLIDER_USE_DEFAULT_CALIBRATION:
        // memcpy(config->x1_calibration, (uint8_t[2]){0, 255}, 2);
        // memcpy(config->x2_calibration, (uint8_t[2]){0, 255}, 2);
        config->x1_calibration[0] = 0;
        config->x1_calibration[1] = 255;
        config->x2_calibration[0] = 0;
        config->x2_calibration[1] = 255;
        config->calibrated        = true;
        break;
    case SLIDER_CALIBRATE:
        config->calibrated = false;
        break;
    }
}

/**
 * @brief Calibrate slider
 *
 */
void slider_calibrate(slider_config_t *config) {
    // log_info("Slider calibration");

    // Slider calibration
    slider_calibrate_axis(0, SLIDER_LEFT, config);
    slider_calibrate_axis(0, SLIDER_RIGHT, config);
    slider_calibrate_axis(1, SLIDER_LEFT, config);
    slider_calibrate_axis(1, SLIDER_RIGHT, config);

    // Check if max and min are swapped
    if (config->x1_calibration[0] < config->x1_calibration[1]) {
        uint8_t temp              = config->x1_calibration[1];
        config->x1_calibration[1] = config->x1_calibration[0];
        config->x1_calibration[0] = temp;
    }
    if (config->x2_calibration[1] < config->x2_calibration[0]) {
        uint8_t temp              = config->x2_calibration[1];
        config->x2_calibration[1] = config->x2_calibration[0];
        config->x2_calibration[0] = temp;
    }

    // log_info("Slider calibration complete");
    // log_info("Slider 1: %u/%u", config->x1_calibration[0], config->x1_calibration[1]);
    // log_info("Slider 2: %u/%u", config->x2_calibration[0], config->x2_calibration[1]);
    _delay_ms(SLIDER_CALIBRATION_DELAY);
    config->calibrated = true;
}

/**
 * @brief Calibrate slider axis
 *
 * @param axis
 * @param config
 */
// void slider_calibrate_axis(uint8_t slider, e_SLIDER_DIR dir, slider_config_t *config) {
//     char *directions[2];
//     directions[SLIDER_LEFT]  = "left";
//     directions[SLIDER_RIGHT] = "right";

//     log_info("Move slider %u %s", slider, directions[dir]);
//     _delay_ms(SLIDER_CALIBRATION_DELAY);
//     log_info("Calibrating...");
//     uint16_t calx = 0;
//     for (int i = 0; i < 100; ++i) {
//         slider_t slider_values = slider_read_raw();
//         if (slider == 1) {
//             calx += slider_values.raw_x1;
//         } else if (slider == 2) {
//             calx += slider_values.raw_x2;
//         }
//         calx += slider_values.raw_x1;
//         _delay_ms(10);
//     }
//     calx /= 100;

//     // Store calibration value
//     switch (slider) {
//     case 1:
//         config->x1_calibration[dir] = calx;
//         break;
//     case 2:
//         config->x2_calibration[dir] = calx;
//         break;
//     }
// }

/**
 * @brief Read slider values
 *
 * @param config
 */
slider_t slider_read(slider_config_t *config) {
    slider_t position = slider_read_raw();
    position.x1       = slider_adjust(position.raw_x1, config->x1_calibration);
    position.x2       = slider_adjust(position.raw_x2, config->x2_calibration);
    return position;
}

/**
 * @brief Read slider raw values
 *
 */
slider_t slider_read_raw(void) {
    uint16_t slider_raw = adc_read();
    slider_t position   = {
        .x1 = 0,
        .x2 = 0,

        .raw_x1 = slider_raw & 0xFF,
        .raw_x2 = (slider_raw >> 8) & 0xFF,
    };
    return position;
}

/**
 * @brief Adjust slider value
 *
 * @param raw
 * @param config
 * @return uint8_t
 */
uint8_t slider_adjust(uint8_t raw, uint8_t calibration[2]) {
    uint8_t min = calibration[0];
    uint8_t max = calibration[1];

    if (raw < min) {
        return min;
    } else if (raw > max) {
        return max;
    } else {
        return map_uint8(raw, min, max, SLIDER_MIN, SLIDER_MAX);
    }
}

/**
 * @brief Map uint8_t
 *
 */
uint8_t map_uint8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
