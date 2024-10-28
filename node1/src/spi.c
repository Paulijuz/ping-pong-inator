/**
 * @file spi.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-10-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/spi.h"
// #include "uart.h"

#include <stdio.h>
#include <avr/io.h>

void spi_init_master(void) {
    DDRB |= (1 << DDB5) | (1 << DDB7) | (1 << DDB4); // Set MOSI and SCK output, all others input
    SPCR  = (1 << SPE)  | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master, set clock rate fck/16

    spi_disable_slave();
}
void spi_master_transmit(char data) {
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF))) {
        // pass
    }
}

char spi_master_receive(void) {
    spi_master_transmit(0x00);
    return SPDR;
}

void spi_enable_slave(void) {
    PORTB &= ~(1 << PORTB4);
}

void spi_disable_slave(void) {
    PORTB |= (1 << PORTB4);
}
