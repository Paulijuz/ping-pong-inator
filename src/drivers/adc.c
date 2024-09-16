/**
 * @file adc.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief Driver for MAX156 ADC, simple conversion and reading
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/adc.h"

/**
 * @brief Initialize ADC by setting PD5 on MCU as PWM output
 *
 */
void adc_init() {
    DDRD |= (1 << DDD5);                   // Set PD5 as output

    OCR1A = (1 << 0);                      // Set TOP

    TCCR1A |= (1 << COM1A0);               // Compare Output Mode

    TCCR1A |= (1 << WGM10) | (1 << WGM11); // Enable Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << WGM13); // Enable Fast PWM

    TCCR1B |= (1 << CS10);                 // Set prescaler to 1 (default no timer)
}

/**
 * @brief Read ADC values.
 *
 * @return uint16_t. X-axis as the upper 8 bits, and Y-axis as lower 8 bits.
 */
void adc_read(uint16_t *low, uint16_t *high) {
    // Write to base (prevents double write for 16 bit address)
    volatile uint8_t *base = (uint8_t *)0x1400;
    *base                  = 0xFF;
    _delay_us(4);

    // Read from full (reads both x and y from ADC)
    volatile uint16_t *full = (uint16_t *)0x1400;
    *low                    = *full;
    *high                   = *full;
    // volatile int16_t output_x = output & 0xFF;
    // volatile int16_t output_y = (output >> 8) & 0xFF;
}
