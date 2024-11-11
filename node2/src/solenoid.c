/**
 * @file solenoid.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-11-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "sam.h"
#include "solenoid.h"

#define SOLENOID_PIO PIOA
#define SOLENOID_PIN PIO_PA16

typedef enum solenoid_state_e {
    SOLENOID_READY,
    SOLENOID_FIRED,
    SOLENOID_COOLDOWN,
} solenoid_state_t;

solenoid_state_t solenoid_state = SOLENOID_READY;

void solenoid_init(void) {
    // Solneoid pin configuration

    SOLENOID_PIO->PIO_WPMR = PIO_WPMR_WPKEY(0x50494F); // Ensure write protection is disabled

    SOLENOID_PIO->PIO_PER |= SOLENOID_PIN; // Ensure PIO is enabled for the solenoid pin
    SOLENOID_PIO->PIO_OER |= SOLENOID_PIN; // Set solenoid pin to be an output

    SOLENOID_PIO->PIO_SODR = SOLENOID_PIN; // Set solenoid pin to high (relay is active low)

    // Solenoid timer configuration

    NVIC_EnableIRQ(TC0_IRQn); // Enable timer interrupt for TC

    PMC->PMC_WPMR = PMC_WPMR_WPKEY(0x504D43); // Ensure write protection is disabled
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC0); // Enable PMC for TC0

    TC0->TC_WPMR = TC_WPMR_WPKEY(0x54494D); // Ensure write protection is disabled

    // Set the channel to wavemode, clock div to 128 and automatically stop on RC compare.
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_CPCSTOP;

    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; // Enable timer interrupt
    TC0->TC_CHANNEL[0].TC_IMR = TC_IMR_CPCS; // Set mask for timer interrupt

    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; // Enable clock for timer
}

void solenoid_fire(void) {
    if (solenoid_state != SOLENOID_READY) {
        return;
    }

    solenoid_state = SOLENOID_FIRED;

    printf("Solenoid fired!\r\n");

    SOLENOID_PIO->PIO_CODR    = SOLENOID_PIN; // Set low to enable solenoid relay
    
    TC0->TC_CHANNEL[0].TC_RC  = SOLENOID_FIRE_TIME_RC; // Set deactivation timer length
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG; // Start deactivation timer
}

static void solenoid_unfire() {
    printf("Solenoid unfired!\r\n");
    
    solenoid_state = SOLENOID_COOLDOWN;

    SOLENOID_PIO->PIO_SODR = SOLENOID_PIN; // Set high to disable solenoid relay

    TC0->TC_CHANNEL[0].TC_RC  = SOLENOID_COOLDOWN_TIME_RC; // Set cooldown timer length
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG; // Start cooldown timer
}

void TC0_Handler(void) {
    TC0->TC_CHANNEL[0].TC_SR; // Read TC0 Status register to clear interrupt

    switch (solenoid_state) {
        case SOLENOID_READY:
            printf("Solenoid should not be ready!\r\n");
            break;
        case SOLENOID_FIRED:
            solenoid_unfire();
            break;
        case SOLENOID_COOLDOWN:
            solenoid_state = SOLENOID_READY;
            printf("Solenoid ready!\r\n");
            break;
        default:
            printf("This should no be possible...\r\n");
            break;
    }
}