#include <avr/io.h>
#include <stdbool.h>

void button_init() {
    // Set PB0 and PB1 as inputs.
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);
}

bool button_left_held(void) {
    return PINB & (1 << PB1);
}

bool button_right_held(void) {
    return PINB & (1 << PB0);
}

bool button_left_pressed(void) {
    static bool last_state = false;
    bool        current_state = button_left_held();
    bool        pressed = !last_state && current_state;
    last_state = current_state;
    return pressed;
}

bool button_right_pressed(void) {
    static bool last_state = false;
    bool        current_state = button_right_held();
    bool        pressed = !last_state && current_state;
    last_state = current_state;
    return pressed;
}