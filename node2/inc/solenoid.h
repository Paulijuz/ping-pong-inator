/**
 * @file solenoid.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-11-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SOLENOID_H
#define SOLENOID_H

#include "sam.h"
#include "defines.h"
#include "time.h"

#define SOLENOID_FIRE_TIME_MS     200
#define SOLENOID_COOLDOWN_TIME_MS 200

#define SOLENOID_CLOCK_HZ         (F_CPU/128) // Don't touch this
#define SOLENOID_FIRE_TIME_RC     (SOLENOID_CLOCK_HZ * SOLENOID_FIRE_TIME_MS     / 1000) // or this
#define SOLENOID_COOLDOWN_TIME_RC (SOLENOID_CLOCK_HZ * SOLENOID_COOLDOWN_TIME_MS / 1000) // or this

void solenoid_init(void);
void solenoid_fire(void);

#endif // SOLENOID_H