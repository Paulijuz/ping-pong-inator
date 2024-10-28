/**
 * @file servo.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVO_H
#define SERVO_H

#define PWM_DUTY_MIN 0.9/20.0f
#define PWM_DUTY_MAX 2.1/20.0f

void servo_set_pos(float pos);

#endif // SERVO_H