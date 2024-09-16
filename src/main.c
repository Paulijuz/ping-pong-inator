/**
 * @file main.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "defines.h"
#include "drivers/uart.h"
#include "sram_test.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // DDRB       = 0xff;
    // int status = 1;
    // PORTB      = status;

    MCUCR |= 1 << SRE;

    uart_init(9600);

    // while (1) {
    //     SRAM_test();
    //     _delay_ms(10000);
    // }

    while (1) {
        for (int i = 0x1400; i<=0x1744; i++) {
            *(int *)i = 0xFF;
            _delay_ms(1);
        }
        for (int i = 0x1800; i<=0x1FFF; i++) {
            *(int *)i = 0xFF;
            _delay_ms(1);
        }
    }

    // while (1) {
    //     char data = uart_receive_char();
    //     if (data == 'e') {
    //         PORTB  = status;
    //         status = !status;
    //     }
    //     printf("%c", data);
    // }

    return 0;
}
