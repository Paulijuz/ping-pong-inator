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

uint8_t mcp_read(uint8_t address) {
    spi_enable_slave();
    spi_master_transmit(MCP_READ);
    spi_master_transmit(address);
    uint8_t data = spi_master_receive();
    spi_disable_slave();
    return data;
}

// void mcp_read_rx_buffer(uint8_t buffer, uint8_t* data, int length) {
//     spi_enable_slave();

//     spi_master_transmit(MCP_READ_RX_BUFFER(buffer));
//     uint8_t data = spi_master_receive();

//     spi_disable_slave();

//     return data;
// }

void mcp_write(uint8_t address, uint8_t data) {
    spi_enable_slave();
    spi_master_transmit(MCP_WRITE);
    spi_master_transmit(address);
    spi_master_transmit(data);
    spi_disable_slave();
}

// void mcp_load_tx_buffer(uint8_t buffer, uint8_t* data, int length) {
//     spi_enable_slave();

//     spi_master_transmit(MCP_LOAD_TX_BUFFER(buffer));
   
//     for (int i = 0; i < length; i++) {
//         spi_master_transmit(data[i]);
//     }

//     spi_disable_slave();
// }

void mcp_request_to_send(uint8_t channels) {
    spi_enable_slave();
    spi_master_transmit(MCP_REQUEST_TO_SEND(channels));
    spi_disable_slave();
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    spi_enable_slave();
    spi_master_transmit(MCP_BIT_MODIFY);
    spi_master_transmit(address);
    spi_master_transmit(mask);
    spi_master_transmit(data);
    spi_disable_slave();
}


uint8_t mcp_read_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_READ_STATUS);
    uint8_t status = spi_master_receive();
    spi_master_receive(); // The MCP will send a copy of status, but we will simply ignore it.

    spi_disable_slave();

    return status;
}

uint8_t mcp_rx_status(void) {
    spi_enable_slave();

    spi_master_transmit(MCP_RX_STATUS);
    uint8_t status = spi_master_receive();
    spi_master_receive(); // The MCP will send a copy of status, but we will simply ignore it.

    spi_disable_slave();

    return status;
}
