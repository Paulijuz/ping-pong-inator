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
        log_warning("Motor speed out of bounds: %f", speed);
        return;
    }

    pwm_set_duty_cycle(speed > 0 ? speed : -speed, MOTOR_SPEED_CHANNEL);

    if (speed < 0) {
        MOTOR_DIR_PIO->PIO_SODR = MOTOR_DIR_PIN;
    } else {
        MOTOR_DIR_PIO->PIO_CODR = MOTOR_DIR_PIN;
    }
}
