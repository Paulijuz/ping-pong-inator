/**
 * @file pwm.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "sam.h"
#include "pwm.h"

void pwm_init(float initial_duty_cycle, uint64_t portb_pin, uint64_t channel) {
    PIOB->PIO_WPMR  = (0x50494F << 8);
    PIOB->PIO_PDR  |= 1 << portb_pin;
    PIOB->PIO_OER  |= 1 << portb_pin;
    PIOB->PIO_ABSR |= 1 << portb_pin;

    PMC-> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_PWM);

    PWM->PWM_WPCR = (0x50574D << 8);
    PWM->PWM_ENA |= 1 << channel;
    PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_128 | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[channel].PWM_CPRD = PWM_PERIOD;
    PWM->PWM_CH_NUM[channel].PWM_CDTY = initial_duty_cycle*PWM_PERIOD; 
}

void pwm_set_duty_cycle(float duty_cycle, int channel) {
    // printf(" %d", (int)(PWM_PERIOD*duty_cycle));
    PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = PWM_PERIOD*duty_cycle;
}