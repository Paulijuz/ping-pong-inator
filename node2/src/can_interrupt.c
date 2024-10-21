/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "can_controller.h"

#define DEBUG_RX_INTERRUPT 1
#define DEBUG_TX_INTERRUPT 0
#define DEBUG_BUS_ERROR    0

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
  char can_sr = CAN0->CAN_SR;

  // Disable global interrupts
  // cli();

  // RX interrupt
  if (can_sr & (CAN_SR_MB1 | CAN_SR_MB2)) // Only mailbox 1 and 2 specified for receiving
  {
    CAN_MESSAGE message;
    uint8_t     status = 0;

    // Mailbox 1 event
    if (can_sr & CAN_SR_MB1) {
      status |= can_receive(&message, 1);

    }
    // Mailbox 2 event
    else if (can_sr & CAN_SR_MB2) {
      status |= can_receive(&message, 2);
    }

    // Incorrect event
    else {
      printf("CAN0 message arrived in non-used mailbox\n\r");
    }

    if (status != 0) {
      // Somehow faster CAN messages prevent mailbox busy errors??
      printf("CAN0 Mailboxes busy\n\r");
      return;
    }

    if (DEBUG_RX_INTERRUPT) {
      // printf("message id: %d\n\r", message.id);
      printf("%d -> ", message.data_length);
      if (message.data_length > 2) {
        printf("message too long, id: %d\n\r", message.id);
      }
      for (int i = 0; i < message.data_length; i++) {
        printf("%d ", (uint8_t)message.data[i]);
      }
      printf("\n\r");
    }
  }

  if (can_sr & CAN_SR_MB0) {
    if (DEBUG_TX_INTERRUPT)
      printf("CAN0 MB0 ready to send \n\r");

    // Disable interrupt
    CAN0->CAN_IDR = CAN_IER_MB0;
  }

  if (can_sr & CAN_SR_ERRP) {
    if (DEBUG_BUS_ERROR)
      printf("CAN0 ERRP error\n\r");
  }
  if (can_sr & CAN_SR_TOVF) {
    if (DEBUG_BUS_ERROR)
      printf("CAN0 timer overflow\n\r");
  }

  NVIC_ClearPendingIRQ(ID_CAN0);

  // sei();
}
