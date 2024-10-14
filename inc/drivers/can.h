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

#include <stdbool.h>

typedef struct can_message_s {
    uint16_t id;
    char data[8];
    uint8_t length;
} can_message_s;

void can_init(void);
void can_transmit(can_message_s* message);
bool can_receive(can_message_s* message);

#endif //CAN_H