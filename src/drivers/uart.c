/**
 * @file uart.c
 * @author Johansson, Juzenas & Stensrud
 * @brief Uart driver.
 * @version 0.1
 * @date 2024-09-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Initalizes usart 0.
 */
void uart_init(unsigned int baud_rate) {
    // TODO: Add option to init uart 1.
    baud_rate = F_CPU / 16 / baud_rate - 1;

    // Set baud rate.
    UBRR0H = baud_rate >> 8;
    UBRR0L = baud_rate;

    // Enable recive and transmit.
    UCSR0B = (1 << RXEN0) | // Enable RX.
             (1 << TXEN0);  // Enable TX.

    //  (1 << RXCIE0); // Enable RX interrupt.
    //   (1 << TXCIE0); // Enable TX interrupt.

    // Set frame format.
    UCSR0C = (1 << URSEL0) | // Enable read/write for UCSRC and disable read/write for UBRRH.
             (1 << USBS0) |  // Set number of stop bits to 2.
             (3 << UCSZ00);  // Set number of data bits to 8.
}
/**
 * @brief Transmits a single char over uart 0.
 *
 * @param data
 */
void uart_transmit_char(unsigned char data) {
    // Wait for transmit buffer to be empty.
    // We do this by checking wheter or not the data
    // register empty flag (UDRE0) in uart control and
    // status register A (UCSR0A) is 1.
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Set transmit register to data.
    UDR0 = data;
}

char uart_receive_char() {
    // Wait for uart recieve to be complete.
    while (!(UCSR0A & (1 << RXC0)))
        ;

    // Read recieved data.
    return UDR0;
}