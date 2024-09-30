/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */

#ifndef FONTS_H_
#define FONTS_H_

#include <avr/pgmspace.h>

extern const unsigned char PROGMEM font8[95][8];
extern const unsigned char PROGMEM font5[95][5];
extern const unsigned char PROGMEM font4[95][4];

// clang-format off
// const unsigned char PROGMEM const **fonts[] = {
//     font8,
//     font5,
//     font4
// };

typedef enum font_size_e {
    FONT_8,
    FONT_5,
    FONT_4,
} font_size_e;
// clang-format on

/**
 * @brief Font configuration
 *
 */
typedef struct font_config_s {
    font_size_e font;
    uint8_t     font_width;
} font_config_t;

#endif /* FONTS_H_ */
