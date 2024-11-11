/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */

#include <stdio.h>

#include "sam.h"
#include "can_interrupt.h"
#include "can_controller.h"
#include "logger.h"
#include "motor.h"
#include "servo.h"

#define DEBUG_RX_INTERRUPT 1
#define DEBUG_TX_INTERRUPT 0
#define DEBUG_BUS_ERROR    0

can_handler_buffer_entry_t can_handler_buffer[CAN_HANDLER_BUFFER_SIZE] = {0};

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval
 */
void CAN0_Handler(void) {
    // if (DEBUG_RX_INTERRUPT)
    //   printf("CAN0 interrupt\n\r");
    char can_sr = CAN0->CAN_SR; // CAN Status Register

    // Disable global interrupts
    // cli();
    // __disable_irq();
    // NVIC_DisableIRQ(ID_CAN0);

    // RX interrupt
    uint8_t can_mailbox_interrupt = can_sr & (CAN_SR_MB1 | CAN_SR_MB2); // Only mailbox 1 and 2 specified for receiving
    if (can_mailbox_interrupt) {
        CAN_MESSAGE message;
        uint8_t     status = 0;

        // Check which mailbox received the message
        if (can_sr & CAN_SR_MB1) {
            status |= can_receive(&message, 1);                      // Mailbox 1 event
        } else if (can_sr & CAN_SR_MB2) {
            status |= can_receive(&message, 2);                      // Mailbox 2 event
        } else {
            log_warning("CAN0 message arrived in non-used mailbox"); // Incorrect event
        }

        // Check error status
        if (status != 0) {
            // Somehow faster CAN messages prevent mailbox busy errors??
            log_warning("CAN0 mailboxes busy"); // Incorrect event
            return;
        }

        if (DEBUG_RX_INTERRUPT) {
            if (message.data_length > 2) {
                log_warning("CAN0 message too long, id: %d", message.id);
            }

            int8_t data[message.data_length];
            for (int i = 0; i < message.data_length; i++) {
                data[i] = *((int8_t *)&message.data[i]);
            }
            log_debug("CAN0 message received. ID: %d, Data: %s", message.id, data);
            // printf("ID: %d -> ", message.id);
            // // printf("%d -> ", message.data_length);
            // if (message.data_length > 2) {
            //     printf("message too long, id: %d\n\r", message.id);
            // }
            // for (int i = 0; i < message.data_length; i++) {
            //     printf("%d ", *((int8_t *)&message.data[i]));
            // }
            // printf("\n\r");
        }

        for (int i = 0; i < CAN_HANDLER_BUFFER_SIZE; i++) {
            if (message.id != can_handler_buffer[i].id || !can_handler_buffer[i].handler)
                continue;

            can_handler_buffer[i].handler(&message);
        }
    }

    if (can_sr & CAN_SR_MB0) {
        if (DEBUG_TX_INTERRUPT) {
            log_debug("CAN0 mailbox 0 ready to send");
            // printf("CAN0 MB0 ready to send \n\r");
        }

        // Disable interrupt
        // CAN0->CAN_IDR = CAN_IER_MB0;
    }

    if (can_sr & CAN_SR_ERRP) {
        if (DEBUG_BUS_ERROR) {
            log_error("CAN0 ERRP error");
            // printf("CAN0 ERRP error\n\r");
        }
    }
    if (can_sr & CAN_SR_TOVF) {
        if (DEBUG_BUS_ERROR) {
            log_error("CAN0 timer overflow");
            // printf("CAN0 timer overflow\n\r");
        }
    }

    // NVIC_ClearPendingIRQ(ID_CAN0);

    // Enable global interrupts
    // sei();
    // __enable_irq();
    // NVIC_EnableIRQ(ID_CAN0);
}

int can_register_handler(int id, void (*handler)(CAN_MESSAGE *)) {
    for (int i = 0; i < CAN_HANDLER_BUFFER_SIZE; i++) {
        if (can_handler_buffer[i].handler)
            continue;

        can_handler_buffer[i].id      = id;
        can_handler_buffer[i].handler = handler;

        return id;
    }

    return -1;
}
