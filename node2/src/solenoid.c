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

#include "solenoid.h"

#define SOLENOID_PIO PIOA
#define SOLENOID_PIN PIO_PA16

void solenoid_init(void) {
    // Enable PMC for the pin
    // PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_PIOA);

    // Ensure write protection is disabled
    SOLENOID_PIO->PIO_WPMR = 0x50494F << 8;

    SOLENOID_PIO->PIO_PER |= SOLENOID_PIN;
    SOLENOID_PIO->PIO_OER |= SOLENOID_PIN;

    SOLENOID_PIO->PIO_SODR = SOLENOID_PIN;
}

void solenoid_fire(void) {
    SOLENOID_PIO->PIO_CODR = SOLENOID_PIN;
    time_spinFor(msecs(200)); // TODO: Change this to a timer interrupt so it doesn't block
    SOLENOID_PIO->PIO_SODR = SOLENOID_PIN;
}
