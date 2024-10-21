/**
 * @file can.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/mcp.h"
#include "drivers/can.h"

#include <stdio.h>

void can_init(void) {
    mcp_reset();
    mcp_write(MCP_CNF1,(0 << MCP_CNF1_SJW) | (4 << MCP_CNF1_BRP)); //Set SJW to 1*TQ and BRP to 4
    mcp_write(MCP_CNF2, (1 << MCP_CNF2_BTLMODE) | (0 << MCP_CNF2_SAM) | (6 << MCP_CNF2_PHSEG1) | (1 << MCP_CNF2_PRSEG));
    mcp_write(MCP_CNF3, (5 << MCP_CNF3_PHSEG2));

    mcp_bit_modify(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_NORMAL);
    mcp_bit_modify(MCP_RXB0CTRL, MCP_RXB0CTRL_RXM_MASK, MCP_RXB0CTRL_RXM_FILTER_OFF);
    
}

void can_transmit(can_message_s* message) {
    mcp_write(MCP_TXB0DLC, message->length);

    mcp_write(MCP_TXB0SIDH, (message->id >> 3) & 0xFF);
    mcp_write(MCP_TXB0SIDL, (message->id & 0b111) << 5);

    for (int i = 0; i < message->length; i++) {
        mcp_write(MCP_TXB0D0 + i, message->data[i]);
    }

    mcp_request_to_send(1);
}

bool can_receive(can_message_s* message) {
    if (!(mcp_read_status() & (1 << MCP_CANINTF_RX0IF))) return false;

    message->length  = mcp_read(MCP_TXB0DLC);
    message->id = ((uint16_t)mcp_read(MCP_RXB0SIDH)) << 3;
    message->id |= ((uint16_t)mcp_read(MCP_RXB0SIDL) >> 5) & 0b111;

    for (int i = 0; i < message->length; i++) {
        message->data[i] = mcp_read(MCP_RXB0D0 + i);
    }

    mcp_bit_modify(MCP_CANINTF, MCP_CANINTF_RX0IF_MASK, 0 << MCP_CANINTF_RX0IF);

    return true;
}


