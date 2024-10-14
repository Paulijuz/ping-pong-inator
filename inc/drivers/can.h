/**
 * @file can.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CAN_H

#define CAN_H


void can_init(void);

void can_transmit(int id, char* data, int length);

int can_receive(char* data, int max_size);




#endif //CAN_H