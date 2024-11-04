/**
 * @file decoder.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sam.h"

void decoder_init() {
    PMC -> PMC_WPMR = (0x504D43 << PMC_WPMR_WPKEY_Pos);
    
    // The id for TC2 is not ID_TC2, but ID_TC6 through 8... THIS TOOK US 7 HOURS TO FIGURE OUT
    // Shout out to Inge Grelland's group for helping us out <3

    PMC -> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC6); //Enable TC2
    PMC -> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC7); 
    PMC -> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC8); 

    PIOC->PIO_WPMR = (0x50494F << 8);

    // Give PC25 and PC26 to peripheral B (TC2).
    PIOC->PIO_PDR  |= PIO_PC26 | PIO_PC25;
    PIOC->PIO_ABSR |= PIO_PC26 | PIO_PC25;

    TC2 -> TC_WPMR = 0x54494D << TC_WPMR_WPKEY_Pos;
    
    // Set mot to position quad decoder.
    TC2 -> TC_BMR  = TC_BMR_QDEN | TC_BMR_POSEN;

    // For both: Use TIOB as trigger and trigger on rising edge.
    // For 0: Use XC0 as external clock.
    TC2 -> TC_CHANNEL[0].TC_CMR = TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG | TC_CMR_TCCLKS_XC0;
    TC2 -> TC_CHANNEL[1].TC_CMR = TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;

    // Start channel 0 and 1.
    TC2 -> TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    TC2 -> TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int decoder_pos() {
    return TC2 -> TC_CHANNEL[0].TC_CV;
}
int decoder_rev() {
    return TC2 -> TC_CHANNEL[1].TC_CV;
}
