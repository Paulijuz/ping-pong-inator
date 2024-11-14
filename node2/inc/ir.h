/**
 * @file ir.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef IR_H
#define IR_H

#include <stdbool.h>
#include "logger.h"

#define IR_HIT_THREASHOLD  500
#define IR_RESET_THRESHOLD 3000
#define IR_HIT_COUNT_MAX 5

void ir_init();
bool ir_hit();

#endif // IR_H
