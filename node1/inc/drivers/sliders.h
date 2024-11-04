/**
 * @file sliders.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SLIDERS_H
#define SLIDERS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "drivers/adc.h"
#include "logger.h"

#define SLIDER_CALIBRATION_DELAY 1500 // ms, delay between calibration steps (for human interaction

// Slider minimum and maximum
#define SLIDER_MIN 0
#define SLIDER_MAX 255

// clang-format off
typedef enum SLIDER_DIR {
    SLIDER_LEFT,
    SLIDER_RIGHT,
} e_SLIDER_DIR;
// clang-format on

typedef enum SLIDER_INITIALIZATION_STATE {
    SLIDER_NO_CALIBRATE,
    SLIDER_USE_DEFAULT_CALIBRATION,
    SLIDER_CALIBRATE
} e_SLIDER_INITIALIZATION_STATE;

/**
 * @brief Struct for slider configuration
 *
 */
typedef struct slider_config_s {
    uint8_t x1_calibration[2];
    uint8_t x2_calibration[2];
    bool    calibrated;
} slider_config_t;

/**
 * @brief Struct for raw and calibrated slider values
 *
 */
typedef struct slider_s {
    int8_t  x1;
    int8_t  x2;

    uint8_t raw_x1;
    uint8_t raw_x2;
} slider_t;

void     slider_init(slider_config_t *config, e_SLIDER_INITIALIZATION_STATE state);
void     slider_calibrate(slider_config_t *config);
void     slider_calibrate_axis(uint8_t slider, e_SLIDER_DIR dir, slider_config_t *config);
slider_t slider_read(slider_config_t *config);
slider_t slider_read_raw(void);
uint8_t  slider_adjust(uint8_t raw, uint8_t calibration[2]);
uint8_t  map_uint8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);

#endif // SLIDERS_H
