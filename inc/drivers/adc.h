#include "defines.h"
#include <util/delay.h>

#include <stdint.h>
#include <avr/io.h>

// void adc_init();
void adc_init();
void adc_read(uint16_t *low, uint16_t *high);
