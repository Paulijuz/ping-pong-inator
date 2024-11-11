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
#include "time.h"

void solenoid_init(void);
void solenoid_fire(void);

#endif // SOLENOID_H
