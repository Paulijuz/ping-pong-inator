/**
 * @file timer_counter.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef TIMER_COUNTER_H
#define TIMER_COUNTER_H

#include <stdint.h>

void timer_counter_init();

int tc_read_motor_pos();
int tc_read_motor_rev();

int16_t read_encoder();

void tc_read_status();

#endif // TIMER_COUNTER_H