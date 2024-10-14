/**
 * @file spi.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SPI_H
#define SPI_H


void spi_init_master(void);
void spi_master_transmit(char cData);
char spi_master_receive(void);
void spi_enable_slave(void);
void spi_disable_slave(void);

#endif //SPI_H