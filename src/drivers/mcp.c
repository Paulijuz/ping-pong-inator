/**
 * @file mcp.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief 
 * @version 0.1
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/mcp.h"

void mcp_reset(void) {
    spi_enable_slave();
    spi_master_transmit(MCP_RESET);
    spi_disable_slave();
}

char mcp_read(char address) {
    spi_enable_slave();
    spi_master_transmit(MCP_READ);
    spi_master_transmit(address);
    char data = spi_master_read();
    spi_disable_slave();
    return data;

}

char mcp_read_rx_buffer(char buffer) {
    spi_enable_slave();

    spi_master_transmit(MCP_READ_RX_BUFFER(buffer));
    char data = spi_master_read();

    spi_disable_slave();

    return data;
}

void mcp_write(char address, char data) {
    spi_enable_slave();
    spi_master_transmit(MCP_WRITE);
    spi_master_transmit(address);
    spi_master_transmit(data);
    spi_disable_slave();
}

void mcp_load_tx_buffer(char buffer, char data) {
    spi_enable_slave();

    spi_master_transmit(MCP_LOAD_TX_BUFFER(buffer));
    spi_master_transmit(data);

    spi_disable_slave();
}

void mcp_request_to_send(char channels) {
    spi_enable_slave();
    spi_master_transmit(MCP_REQUEST_TO_SEND(channels));
    spi_disable_slave();
}

void mcp_bit_modify(char address, char mask, char data) {
    spi_enable_slave();
    spi_master_transmit(MCP_BIT_MODIFY);
    spi_master_transmit(address);
    spi_master_transmit(mask);
    spi_master_transmit(data);
    spi_disable_slave();
}


char mcp_read_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_READ_STATUS);
    char status = spi_master_read();
    spi_master_read(); // The MCP will send a copy of status, but we will simply ignore it.

    spi_disable_slave();

    return status;
}

char mcp_rx_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_RX_STATUS);
    char status = spi_master_read();
    spi_master_read(); // The MCP will send a copy of status, but we will simply ignore it.

    spi_disable_slave();

    return status;
}