/**
 * @file ir.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "ir.h"
#include "sam.h"

void ir_init() {
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_ADC);

    ADC->ADC_WPMR = (0x414443 << 8);
    ADC->ADC_MR   = ADC_MR_FREERUN_ON;
    ADC->ADC_CHER = ADC_CHER_CH0;
    ADC->ADC_CR   = ADC_CR_START;
}

bool ir_hit() {
    static bool can_hit = false;

    uint32_t    adc_value = ADC->ADC_CDR[0];
    if (adc_value > IR_RESET_THRESHOLD) {
        can_hit = true;
    }

    // static int hit_count = 0;
    // log_debug("%d", adc_value);

    if (can_hit && adc_value < IR_HIT_THREASHOLD) {
        // hit_count = min(hit_count + 1, IR_HIT_COUNT_MAX);
        can_hit = false;
        return true;
    } else {
        // hit_count = 0;
    }

    return false;
    // return hit_count >= IR_HIT_COUNT_MAX;
}
