/**
 * @file oled.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "drivers/oled.h"
#include "fonts.h"

#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

uint8_t* oled_disp_buffer_base = OLED_BUFFER_BASE_A;
uint8_t* oled_draw_buffer_base = OLED_BUFFER_BASE_B;
static font_config_t font_config = FONT4_CONFIG;


uint8_t oled_current_line = 0;
uint16_t oled_current_column = 0;

bool oled_buffer_should_flush = false;

/**
 * @brief Write command to OLED
 *
 * @param c
 */
void oled_cmd_write_char(char c) {
    // Write to address base
    *OLED_CMD_BASE = c;
}

/**
 * @brief Initialize OLED (from OLED datasheet, see lab support data)
 *
 */
void oled_init(void) {
    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_OFF);           // display off
    oled_cmd_write_char(OLED_CMD_SET_SEGMENT_REMAP_REVERSE); // segment remap

    oled_cmd_write_char(OLED_CMD_SET_COM_PINS_HW_CONF);      // common pads hardware: alternative
    oled_cmd_write_char(OLED_CMD_SET_COM_PINS_HW_CONF_ALT_DIS);

    oled_cmd_write_char(OLED_CMD_SET_COM_OUTPUT_SCAN_DIR_REV); // common output scan direction:com63~com0 = reversed

    oled_cmd_write_char(OLED_CMD_SET_MULTIPLEX_RATIO);         // multiplex ration mode:63
    oled_cmd_write_char(0x3f);

    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE); // display divide ratio/osc. freq. mode
    oled_cmd_write_char(0x80);

    oled_cmd_write_char(OLED_CMD_SET_CONTRAST); // contrast control
    oled_cmd_write_char(0x50);

    oled_cmd_write_char(OLED_CMD_SET_PRECHARGE_PERIOD); // set pre-charge period
    oled_cmd_write_char(0x21);

    oled_cmd_write_char(OLED_CMD_SET_MEMORY_ADDRESSING_MODE); // Set Memory Addressing Mode
    oled_cmd_write_char(OLED_CMD_MEMORY_ADDRESS_MODE_VERT);

    oled_cmd_write_char(OLED_CMD_SET_VCOMH_DESELECT_LEVEL); // VCOM deselect level mode
    oled_cmd_write_char(OLED_CMD_VCMOH_DESELECT_LEVEL_2);

    oled_cmd_write_char(OLED_CMD_SET_IREF_SELECTION); // master configuration
    oled_cmd_write_char(OLED_CMD_SET_IREF_SELECTION_EXTERNAL);

    oled_cmd_write_char(OLED_CMD_SET_ENTIRE_DISPLAY_ON); // out follows RAM content
    oled_cmd_write_char(OLED_CMD_SET_NORMAL_DISPLAY);    // set normal display
    oled_cmd_write_char(OLED_CMD_SET_DISPLAY_ON);        // display on

    // OCR0 = (80); // Set TOP for timer 0
    // TCCR0 |= (1 << WGM01 | 1 << CS02 | 1 << CS00); // Enable CTC and set clock prescaler to 1024
    // TIMSK |= (1 << OCIE0); // Enable timer interrupt  

    // Set the compare value for 50 Hz interrupt (assuming 4.9152 MHz clock and 1024 prescaler)
    OCR0 = 95;

    // Set CTC mode for Timer 0 (Clear Timer on Compare Match)
    TCCR0 |= (1 << WGM01 | 1 << CS02 | 1 << CS00);

    // Enable Timer 0 Compare Match Interrupt
    TIMSK |= (1 << OCIE0);
}

/**
 * @brief Routes printf to OLED.
 * 
 */
void oled_enable_printf(void) {
    fdevopen(oled_print_char_file, NULL);
}

void oled_reset(void) {}
void oled_home(void) {}

/**
 * @brief Go to line (bytes) on OLED
 *
 * @param line
 */
void oled_goto_line(uint8_t line) {
    oled_current_line = line; 
}

/**
 * @brief Go to column (pixels) on OLED
 * 
 * @param column 
 */

void oled_goto_column(uint8_t column) {

    oled_current_column = column;
}





/**
 * @brief Clear line on OLED
 *
 * @param line
 */
void oled_clear_line(uint8_t line) {
    for (uint8_t i = 0; i < OLED_WIDTH_PIXELS; ++i) {
        *((oled_draw_buffer_base + line) + i*OLED_HEIGHT_BYTES) = 0x00; 
    }
}


void oled_clear_screen() {
    for (int i = 0; i < OLED_BUFFER_SIZE; ++i) {
        *(oled_draw_buffer_base + i) = 0x00;
    }
}


uint8_t* oled_pos(void) {

    return oled_draw_buffer_base + oled_current_line + oled_current_column*8;

}

/**
 * @brief Set font for OLED
 *
 * @param font
 */
void oled_set_font(font_config_t *font) {
    // Set local font configuration
    font_config = *font;
}



void oled_print_char(char c) {
    if (c < 32 || c > 126) {
        c = 126;
    }

    if (oled_current_column + font_config.font_width >= OLED_WIDTH_PIXELS) {
        oled_cursor_increment();
    }
    
    uint8_t* draw_pointer = oled_pos();
 

    // Special case for space
    if (c == 32) {
        for (int i = 0; i < font_config.font_width; ++i) {
            *(draw_pointer + i*OLED_HEIGHT_BYTES) = 0x00;
        }
        oled_cursor_increment();
        return;
    }

    // Read font from program memory
    // FROM: https://www.nongnu.org/avr-libc/user-manual/pgmspace.html
    // memcpy_P((void *)OLED_DATA_BASE, font4[c - 32], 4); // This can be unreliable for some reason?? It was unreliable because out atmega was f**ked!

    const unsigned char(*font_ptr)[95][font_config.font_width] = (const unsigned char(*)[95][font_config.font_width])font_config.font_ptr;
    
    for (int i = 0; i < font_config.font_width; ++i) {
        *(draw_pointer + i*OLED_HEIGHT_BYTES) = pgm_read_byte(&((*font_ptr)[c - 32][i]));
    }

    oled_cursor_increment();
}


void oled_cursor_increment() {
    oled_current_column += font_config.font_width + 1;

    if (oled_current_column >= OLED_WIDTH_PIXELS) {
        oled_current_line++;
        oled_current_line %= OLED_HEIGHT_BYTES;

        oled_current_column = 0;
    }
}

/**
 * @brief Print string to OLED
 *
 * @param str
 */
void oled_print_string(const char *str) {
    while (*str) {
        oled_print_char(*str++);
    }
}

void oled_flush_buffer() {
    oled_buffer_should_flush = false;

    oled_cmd_write_char(OLED_CMD_SET_COLUMN_ADDRESS);
    oled_cmd_write_char(0);
    oled_cmd_write_char(OLED_WIDTH_PIXELS - 1);
    
    oled_cmd_write_char(OLED_CMD_SET_PAGE_ADDRESS);
    oled_cmd_write_char(0);
    oled_cmd_write_char(OLED_HEIGHT_BYTES - 1);

    for (int i = 0; i < OLED_BUFFER_SIZE; i++) {
        *OLED_DATA_BASE = *(oled_disp_buffer_base + i);
    }
}

void oled_flip_buffer() {
    memcpy(oled_disp_buffer_base, oled_draw_buffer_base, OLED_BUFFER_SIZE);
    uint8_t* temp_buffer_flip = oled_disp_buffer_base;
    oled_disp_buffer_base = oled_draw_buffer_base;
    oled_draw_buffer_base = temp_buffer_flip;
}

uint8_t oled_get_line() {
    return oled_current_line;
}

uint16_t oled_get_column() {
    return oled_current_column;
}

int oled_print_char_file(char data, FILE *file) {
    oled_print_char(data);

    return 0;
}

bool oled_should_flush() {
    return oled_buffer_should_flush;
}

void oled_set_contrast(uint8_t contrast) {
    oled_cmd_write_char(OLED_CMD_SET_CONTRAST); // contrast command
    oled_cmd_write_char(contrast); // contrast value
}

ISR(TIMER0_COMP_vect) {
    oled_buffer_should_flush = true;
}
