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


#include "sam.h"

void init_ir() {
    PMC-> PMC_PCR = PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_ADC);
    ADC->ADC_WPMR = (0x414443 << 8);
    // ADC -> ADC_MR = 
    ADC -> ADC_CHER = ADC_CHER_CH0;
    ADC -> ADC_CR = ADC_CR_START;
}