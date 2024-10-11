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


void can_init(void) {
    mcp_reset();
    mcp_bit_modify(MCP_CANCTRL, MCP_CANCTRL_REQOP_MASK, MCP_CANCTRL_REQOP_LOOPBACK);

}

void can_transmit(int id, char* data, int length) {
    mcp_load_tx_buffer(1, 'a');
    mcp_request_to_send(1);

}

int can_recieve(char* data, int max_size) {
    *data = mcp_read_rx_buffer(1);
    return 1;
}


