/**
 * @file pid.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-11-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "controller.h"

bool  should_execute     = true;
float reference_position = 0;

void  controller_init() {
    // Solenoid timer configuration

    NVIC_EnableIRQ(TC3_IRQn);                                       // Enable timer interrupt for TC

    PMC->PMC_WPMR = PMC_WPMR_WPKEY(0x504D43);                       // Ensure write protection is disabled
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_CMD | PMC_PCR_PID(ID_TC3); // Enable PMC for channel 0 on timer 1 == TC(3*timer + channel)

    TC1->TC_WPMR = TC_WPMR_WPKEY(0x54494D);                         // Ensure write protection is disabled

    // Set the channel to wavemode, clock div to 128 and automatically stop on RC compare.
    TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE;

    TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;            // Enable timer interrupt
    TC1->TC_CHANNEL[0].TC_IMR = TC_IMR_CPCS;            // Set mask for timer interrupt

    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;           // Enable clock for timer

    TC1->TC_CHANNEL[0].TC_RC  = CONTROLLER_INTERVAL_RC; // Set deactivation timer length
    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;           // Start deactivation timer

    // TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;           // Start deactivation timer

    reference_position = 0;
}

void TC3_Handler(void) {
    TC1->TC_CHANNEL[0].TC_SR; // Read TC1 Status register to clear interrupt
    should_execute = true;
}

bool controller_should_execute(void) {
    return should_execute;
}
void controller_clear_execute(void) {
    should_execute = false;
}

void controller_set_reference(float reference) {
    reference_position = reference;
}

float controller_get_reference(void) {
    return reference_position;
}

void controller_execute(void) {
    static float error_sum = 0;
    // static float error_prev = 0;

    float reference = controller_get_reference();

    // Error check on input
    if (reference > 1 || reference < -1) {
        log_warning("Reference out of bounds: %f", reference);
        return;
    }

    int   decoder_low = decoder_pos();
    float current_pos = (decoder_low / MOTOR_POS_MAX - 0.5f) * 2;

    float error_curr = reference - current_pos;
    error_sum += error_curr;

    float input = KP * error_curr + (1.0f / CONTROLLER_INTERVAL_MS) * KI * error_sum;
    if (input > 1) {
        input = 1;
    } else if (input < -1) {
        input = -1;
    }
    // log_debug("Pos, Decoder, Integrator, Input: %f, %d, %f, %f", current_pos, decoder_low, error_sum, input);
    // log_debug("Input: %f", input);
    motor_set_speed(input);
    controller_clear_execute();
}
