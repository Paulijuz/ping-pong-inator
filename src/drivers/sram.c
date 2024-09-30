/**
 * @file sram.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <avr/io.h>
#include "drivers/sram.h"

void sram_init() {
    MCUCR |= 1 << SRE; // enable external sram
}