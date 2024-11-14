/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#include "can_controller.h"

#define CAN_HANDLER_BUFFER_SIZE 20

#define CAN_ID_JOYSTICK         1000
#define CAN_ID_BUTTON           500
#define CAN_ID_IR               250

typedef struct can_handler_buffer_entry_s {
    int id;
    void (*handler)(CAN_MESSAGE*);
} can_handler_buffer_entry_t;

void CAN0_Handler(void);
int can_register_handler(int id, void (*handler)(CAN_MESSAGE*));

#endif /* CAN_INTERRUPT_H_ */
