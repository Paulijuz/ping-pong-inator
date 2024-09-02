/**
 * @file main.c
 * @author Theodor Johansson, Paulius Juzenas & William Juul Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0xff;

    while (1) {
        PORTB = 0xff;
        _delay_ms(500);
        PORTB = 0x00;
        _delay_ms(500);
    }

    return 0;
}