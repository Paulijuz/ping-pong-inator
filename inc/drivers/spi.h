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


void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

#endif //SPI_H