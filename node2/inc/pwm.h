/**
 * @file pwm.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PWM_H
#define PWM_H

#define PWM_DUTY_MIN 591
#define PWM_DUTY_MAX 1378
#define PWM_PERIOD 13125


void pwm_init(float initial_duty_cycle);
void pwm_set_duty_cycle(float duty_cycle);


#endif // PWM_H