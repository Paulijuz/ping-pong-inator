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

#define MCP_RESET 0b11000000
#define MCP_READ  0b00000011
#define MCP_WRITE 0b00000010
#define MCP_REQUEST_TO_SEND(channels) (0b10000000 | (channels & 0b111))
#define MCP_BIT_MODIFY 0b00000101
#define MCP_READ_STATUS 0b10100000

#include "drivers/spi.h"


void mcp_reset(void);
char mcp_read(char address);
char mcp_read_rx_buffer(char address);
void mcp_write(char address, char data);
void mcp_load_tx_buffer(char data);
void mcp_request_to_send(char channels);
void mcp_bit_modify(char address, char mask, char data);
char mcp_read_status(void);
char mcp_rx_status(void);

#endif //MCP_H