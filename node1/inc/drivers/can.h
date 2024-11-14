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

/*****************************************************************************/
/* Typedefs                                                                  */
/*****************************************************************************/

#define CAN_ID_JOYSTICK 1000
#define CAN_ID_BUTTON   500
#define CAN_ID_IR       250

/**
 * @brief Struct for CAN message
 *
 */
typedef struct can_message_s {
    uint16_t id;
    char data[8];
    uint8_t length;
} can_message_s;

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/

void can_init(void);
void can_transmit(can_message_s* message);
bool can_receive(can_message_s* message);

#endif //CAN_H
