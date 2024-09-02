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

#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/uart.h"

int main(void) {
    DDRB       = 0xff;
    int status = 1;
    PORTB      = status;

    uart_init(9600);

    while (1) {
        char data = uart_receive_char();
        if (data == 'e') {
            PORTB  = status;
            status = !status;
        }
        uart_transmit_char(data);
    }

    return 0;
}