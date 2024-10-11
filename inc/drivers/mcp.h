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


void mcp_reset(void);
char mcp_read(char adress);
void mcp_write(char adress, char data);
void mcp_load_tx_buffer(char data);
void mcp_request_to_send(void);
void mcp_bit_modify(char adress, char mask, char data);
char mcp_read_status(void);
char mcp_rx_status(void);

#endif //MCP_H