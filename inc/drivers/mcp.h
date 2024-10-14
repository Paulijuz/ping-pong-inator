/**
 * @file mcp.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MCP_H
#define MCP_H

#include "drivers/spi.h"

#include <stdint.h>

#define MCP_RESET                      0b11000000
#define MCP_READ                       0b00000011
#define MCP_READ_RX_BUFFER(buffer)    (0b10010000 | ((buffer & 0b11) << 1))
#define MCP_WRITE                      0b00000010
#define MCP_LOAD_TX_BUFFER(buffer)    (0b01000000 | (buffer & 0b111))
#define MCP_REQUEST_TO_SEND(channels) (0b10000000 | (channels & 0b111))
#define MCP_BIT_MODIFY                 0b00000101
#define MCP_READ_STATUS                0b10100000
#define MCP_RX_STATUS                  0b10110000

#define MCP_CANCTRL                0b00001111
#define MCP_CANCTRL_REQOP_MASK     0b11100000
#define MCP_CANCTRL_REQOP_LOOPBACK (0b010 << 5)

#define MCP_CANINTF             0b00101100
#define MCP_CANINTF_RX0IF       0
#define MCP_CANINTF_RX0IF_MASK (1 << MCP_CANINTF_RX0IF)

#define MCP_RXB0CTRL                0b01100000
#define MCP_RXB0CTRL_RXM_MASK       0b01100000
#define MCP_RXB0CTRL_RXM_FILTER_OFF 0b11<<5

#define MCP_RXB0SIDH 0b01100001
#define MCP_RXB0SIDL 0b01100010
#define MCP_RXB0DLC  0b01100101
#define MCP_RXB0D0   0b01100110

#define MCP_TXB0SIDH 0b00110001
#define MCP_TXB0SIDL 0b00110010
#define MCP_TXB0DLC  0b00110101
#define MCP_TXB0D0   0b00110110

#define MCP_LOAD_TX_BUFFER_0_ID   0
#define MCP_LOAD_TX_BUFFER_0_DATA 1

void mcp_reset(void);
uint8_t mcp_read(uint8_t address);
uint8_t mcp_read_rx_buffer(uint8_t buffer);
void mcp_write(uint8_t address, uint8_t data);
void mcp_load_tx_buffer(uint8_t buffer, uint8_t data);
void mcp_request_to_send(uint8_t channels);
void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
uint8_t mcp_read_status(void);
uint8_t mcp_rx_status(void);

#endif //MCP_H