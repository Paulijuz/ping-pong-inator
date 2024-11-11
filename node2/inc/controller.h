/**
 * @file controller.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>

#include "defines.h"
#include "sam.h"
#include "decoder.h"
#include "motor.h"
#include "logger.h"

#define MOTOR_POS_MAX          2808.0f
#define MOTOR_POS_MIN          0
#define KP                     0.6
#define KI                     1.5
#define T                      0.01

#define CONTROLLER_INTERVAL_MS 100

#define CONTROLLER_CLOCK_HZ    (F_CPU / 128)                                         // Don't touch this
#define CONTROLLER_INTERVAL_RC (CONTROLLER_CLOCK_HZ * CONTROLLER_INTERVAL_MS / 1000) // or this

void  controller_init(void);
void  controller_set_reference(float reference);
float controller_get_reference(void);
void  controller_execute(void);
bool  controller_should_execute(void);
void  controller_clear_execute(void);

#endif // CONTROLLER_H
