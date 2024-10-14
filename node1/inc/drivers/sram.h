/**
 * @file sram.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SRAM_H
#define SRAM_H

#define SRAM_BASE (uint8_t *)0x1800
#define SRAM_END  (uint8_t *)0x1FFF

void sram_init();

#endif // SRAM_H