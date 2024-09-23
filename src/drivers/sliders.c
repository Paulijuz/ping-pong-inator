// /**
//  * @file sliders.c
//  * @author Theodor Johansson, Paulius Juzenas & William Stensrud
//  * @brief
//  * @version 0.1
//  * @date 2024-09-23
//  *
//  * @copyright Copyright (c) 2024
//  *
//  */

// #include "drivers/sliders.h"

// /**
//  * @brief Calibrate slider
//  *
//  */
// void slider_calibrate(slider_config_t *config) {
//     printf("\r\n--- Slider calibration ---\r\n");

//     // Slider 1 min calibration
//     printf("Move slider 1 left\r\n");
//     _delay_ms(SLIDER_CALIBRATION_DELAY);
//     printf("Calibrating...\r\n");
//     uint8_t calx = 0;
//     for (int i = 0; i < 100; ++i) {
//         slider_t slider = slider_read_raw();
//         calx += slider.raw_x1;

//         _delay_ms(10);
//     }
//     config->x1_min = calx / 100;

//     // Slider 1 max calibration
//     printf("Move slider 1 right\r\n");
//     _delay_ms(SLIDER_CALIBRATION_DELAY);
//     printf("Calibrating...\r\n");
//     calx = 0;
//     for (int i = 0; i < 100; ++i) {
//         slider_t slider = slider_read_raw();
//         calx += slider.raw_x1;

//         _delay_ms(10);
//     }
//     config->x1_max = calx / 100;

//     for (int axis = 0; axis < JOYSTICK_ENUM_LENGTH; ++axis) {
//         slider_calibrate_axis(axis, config);
//     }

//     // Check if max and min are swapped
//     if (config->x_config.max < config->x_config.min) {
//         uint8_t temp         = config->x_config.max;
//         config->x_config.max = config->x_config.min;
//         config->x_config.min = temp;
//     }
//     if (config->y_config.max < config->y_config.min) {
//         uint8_t temp         = config->y_config.max;
//         config->y_config.max = config->y_config.min;
//         config->y_config.min = temp;
//     }

//     printf("\r\n--- Calibration complete ---\r\n");
//     printf("Joystick X: %u/%u/%u\r\n", config->x_config.min, config->x_config.center, config->x_config.max);
//     printf("Joystick Y: %u/%u/%u\r\n", config->y_config.min, config->y_config.center, config->y_config.max);
//     _delay_ms(CALIBRATION_DELAY);
//     config->calibrated = true;
// }

// /**
//  * @brief Calibrate slider axis
//  *
//  * @param axis
//  * @param config
//  */
// void slider_calibrate_axis(uint8_t slider, e_SLIDER_DIR dir, slider_config_t *config) {
//     char *directions[2];
//     directions[SLIDER_LEFT]  = "left";
//     directions[SLIDER_RIGHT] = "right";

//     printf("Move slider %u %s\r\n", slider, directions[dir]);
//     _delay_ms(SLIDER_CALIBRATION_DELAY);
//     printf("Calibrating...\r\n");
//     uint8_t calx = 0;
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

// /**
//  * @brief Read slider values
//  *
//  * @param config
//  */
// slider_t slider_read(slider_config_t *config) {
//     slider_t position = slider_read_raw();
//     position.x1       = slider_adjust(position.raw_x1, config);
//     position.x2       = slider_adjust(position.raw_x2, config);
//     return position;
// }

// /**
//  * @brief Read slider raw values
//  *
//  */
// slider_t slider_read_raw(void) {
//     uint16_t slider_raw = adc_read();
//     slider_t position   = {
//         .x1 = 0,
//         .x2 = 0,

//         .raw_x1 = slider_raw & 0xFF,
//         .raw_x2 = (slider_raw >> 8) & 0xFF,
//     };
//     return position;
// }

// /**
//  * @brief Adjust slider value
//  *
//  * @param raw
//  * @param config
//  * @return uint8_t
//  */
// uint8_t slider_adjust(uint8_t raw, slider_config_t *config) {
//     if (raw < config->min) {
//         return 0;
//     } else if (raw > config->max) {
//         return 100;
//     } else {
//         return (raw - config->min) * 100 / (config->max - config->min);
//     }
// }

// /**
//  * @brief Map uint8_t
//  *
//  */
// uint8_t map_uint8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }
