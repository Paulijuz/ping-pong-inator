#include "sam.h"
#include "pwm.h"
#include "servo.h"
#include "decoder.h"
#include "logger.h"

#define MOTOR_SPEED_CHANNEL 0
#define MOTOR_SPEED_PIN     12

#define MOTOR_DIR_PIO PIOC
#define MOTOR_DIR_PIN PIO_PC23

float map_float(uint32_t val, uint32_t in_min, uint32_t in_max, float out_min, float out_max);

void motor_init() {
    pwm_init(0, MOTOR_SPEED_PIN, MOTOR_SPEED_CHANNEL);

    MOTOR_DIR_PIO->PIO_OER |=  MOTOR_DIR_PIN;
    MOTOR_DIR_PIO->PIO_PER |=  MOTOR_DIR_PIN;
}

void motor_set_speed(float speed) {
    if (speed > 1 || speed < -1) {
        return;
    }

    pwm_set_duty_cycle(speed > 0 ? speed : -speed, MOTOR_SPEED_CHANNEL);

    if (speed < 0) {
        MOTOR_DIR_PIO->PIO_SODR = MOTOR_DIR_PIN;
    } else {
        MOTOR_DIR_PIO->PIO_CODR = MOTOR_DIR_PIN;
    }
}

void motor_set_position(float position) {
    // static float error_prev = 0;
    static float error_curr = 0;
    static float error_sum  = 0;

    if (position > 1 || position < -1) {
        return;
    }

#define MOTOR_POS_MAX 2808.0f
#define MOTOR_POS_MIN 0

    // PD-controller
    // uint32_t decoder_low      = decoder_pos();
    // uint32_t decoder_high     = decoder_rev();
    // uint64_t decoder_position = ((decoder_high << 32) & 0xFFFF0000) | decoder_low;
    // float    current_pos      = map_float(decoder_position, MOTOR_POS_MIN, MOTOR_POS_MAX, -1, 1);

    int   decoder_low = decoder_pos();
    float current_pos = (decoder_low / MOTOR_POS_MAX - 0.5f) * 2;
    // float current_pos = map_float(decoder_low, MOTOR_POS_MIN, MOTOR_POS_MAX, -1, 1);
    // log_debug("Current pos: %f", current_pos);

    //
    // error_prev = error_curr;
    error_curr = position - current_pos;
    error_sum += error_curr;

#define KP 0.6
#define KI 0.4
#define T  0.01

    float input = KP * error_curr + T * KI * error_sum;
    log_debug("Pos, Decoder, Integrator, Input: %f, %d, %f, %f", position, decoder_low, error_sum, input);
    // log_debug("Input: %f", input);
    motor_set_speed(input);

    // float error = position - servo_get_pos();
}

float map_float(uint32_t val, uint32_t in_min, uint32_t in_max, float out_min, float out_max) {
    uint32_t input     = val - in_min;
    uint32_t in_range  = in_max - in_min;
    float    out_range = out_max - out_min;
    float    divisor   = out_range / in_range; // Using float might not be necessary, as flooring the division might be enough given it is done in the right
                                               // order: https://stackoverflow.com/a/5732117
    float mapped = out_min + divisor * input;

    return mapped;
}
