/**
 * @file uart.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief UART driver for ATmega162
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>

void uart_init(unsigned int baud_rate);
int uart_transmit_char(char data);
int uart_transmit_char_file(char data, FILE *file);
char uart_receive_char();
