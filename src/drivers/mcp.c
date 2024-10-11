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

char mcp_read_rx_buffer(char buffer) {
    spi_enable_slave();

    spi_master_transmit(MCP_LOAD_RX(buffer));
    char data = spi_master_receive();

    spi_disable_slave();

    return data;
}

void mcp_load_tx_buffer(char buffer, char data) {
    spi_enable_slave();

    spi_master_transmit(MCP_LOAD_TX_BUFFER(buffer));
    spi_master_transmit(data);

    spi_disable_slave();
}

char mcp_read_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_READ_STATUS);
    char status = spi_master_read();
    char status_copy = spi_master_read();

    spi_disable_slave();

    return status;
}

char mcp_rx_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_RX_STATUS);
    char status = spi_master_read();
    char status_copy = spi_master_read();

    spi_disable_slave();

    return status;
}