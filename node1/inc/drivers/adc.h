/**
 * @file adc.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief ADC driver
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef ADC_H
#define ADC_H

#include "defines.h" // Required for F_CPU, used in util/delay.h
#include <util/delay.h>

#include <stdint.h>
#include <avr/io.h>

void     adc_init(void);
uint32_t adc_read(void);

#endif // ADC_H
