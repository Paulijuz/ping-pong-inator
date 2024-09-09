#include <stdio.h>

void uart_init(unsigned int baud_rate);
int uart_transmit_char(char data);
int uart_transmit_char_file(char data, FILE *file);
char uart_receive_char();