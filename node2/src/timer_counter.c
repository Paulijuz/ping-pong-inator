/**
 * @file timer_counter.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "sam.h"'
#include "time.h"

void timer_counter_init() {

    // PMC -> PMC_PCER0 |= PMC_PCER0_PID29;
    // PIOC -> PIO_PER = 0xff << 1;
    // PIOC -> PIO_IFER = 0xff << 1;
    // PIOC -> PIO_IFER = 0xff << 1;
    // PIOC -> PIO_PUDR = 0xff << 1;

    PIOC->PIO_WPMR = (0x50494F << 8);
    PIOC->PIO_PDR |= PIO_PC26 | PIO_PC25;
    PIOC->PIO_ODR |=  PIO_PC26 | PIO_PC25;
    PIOC->PIO_ABSR |=  PIO_PC26 | PIO_PC25;


    PMC -> PMC_WPMR = (0x504D43 << PMC_WPMR_WPKEY_Pos);
    PMC -> PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC2); //Enable TC2

    TC2 -> TC_WPMR = 0x54494D << TC_WPMR_WPKEY_Pos;
    // TC2 -> TC_BMR |= 0b1111111111111111111111111;
    TC2 -> TC_BMR |= TC_BMR_QDEN;
    TC2 -> TC_BMR |= TC_BMR_POSEN;
    printf("BMR is: %x", TC2->TC_BMR);


    TC2 -> TC_CHANNEL[0].TC_CMR &= ~(TC_CMR_WAVE);
    TC2 -> TC_CHANNEL[1].TC_CMR &= ~(TC_CMR_WAVE);

    TC2 -> TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;
    TC2 -> TC_CHANNEL[1].TC_CMR |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;





    // PMC -> PMC_PCER0 |= PMC_PCER0_PID29;
    
    // TC2 -> TC_WPMR = 0x54494D << TC_WPMR_WPKEY_Pos;

    // TC2 -> TC_BMR |= TC_BMR_QDEN | TC_BMR_SPEEDEN;
    // TC2 -> TC_CHANNEL[2].TC_RC = 20;
    // TC2 -> TC_CHANNEL[2].TC_CMR |= TC_CMR_WAVE;
    // TC2 -> TC_CHANNEL[2].TC_CMR |= TC_CMR_TCCLKS_XC0 | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPC_TOGGLE; //USIKKER PÅ OM XC0 skal være med her


    // TC2 -> TC_CHANNEL[0].TC_CMR &= ~(TC_CMR_WAVE);
    // TC2 -> TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0 | TC_CMR_ABETRG | TC_CMR_ETRGEDG_RISING | TC_CMR_LDRA_RISING; // WAVE = 0

    // //TRENGER NOK BARE EN AV DISSE
    // TC2 -> TC_CHANNEL[2].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;   
    // TC2 -> TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG; 
    }


int16_t read_encoder() {
    uint8_t low_byte, high_byte;

    PIOD -> PIO_CODR = PIO_PER_P0 | PIO_PER_P2;
    time_spinFor(usecs(100));
    high_byte = (PIOC->PIO_PDSR >>1) & (0xff);
    PIOD -> PIO_SODR = PIO_PER_P2;
    time_spinFor(usecs(100));
    low_byte = (PIOC->PIO_PDSR>>1) & (0xff);

    PIOD->PIO_SODR = PIO_PER_P0;

    int16_t encoder_value = (high_byte<<8) & 0xff00 | (low_byte);
    printf("Encoder value: %x\n\r", encoder_value);
    return encoder_value;

}


int tc_read_motor_pos() {
    return TC2 -> TC_CHANNEL[0].TC_CV;
}
int tc_read_motor_rev() {
    return TC2 -> TC_CHANNEL[1].TC_CV;
}

void tc_read_status() {
    printf("BMR: %x\n\r", TC2 -> TC_BMR);
    printf("CH0 CMR: %x\n\r", TC2 -> TC_CHANNEL[0].TC_CMR);
    printf("CH1 CMR: %x\n\r", TC2 -> TC_CHANNEL[1].TC_CMR);
}