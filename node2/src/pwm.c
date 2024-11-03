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




void pwm_init(float initial_duty_cycle) {
    PIOB->PIO_WPMR = (0x50494F << 8);
    PIOB->PIO_PDR |= PIO_PB13;
    PIOB->PIO_OER |=  PIO_PB13;
    PIOB->PIO_ABSR |=  PIO_PB13;

    PMC-> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_PWM);

    PWM->PWM_WPCR = (0x50574D << 8);
    PWM->PWM_ENA = PWM_SR_CHID1;
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_128 | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_PERIOD;
    PWM->PWM_CH_NUM[1].PWM_CDTY = initial_duty_cycle; // FIX LATER 


}

void pwm_set_duty_cycle(float duty_cycle) {
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = duty_cycle*PWM_PERIOD;
}