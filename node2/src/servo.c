/**
 * @file servo.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "pwm.h"
#include "servo.h"

void servo_set_pos(float pos) {
    if (pos < 0 || pos > 1) {
        return;
    }
    pwm_set_duty_cycle(PWM_DUTY_MIN + (PWM_DUTY_MAX-PWM_DUTY_MIN)*pos);
}