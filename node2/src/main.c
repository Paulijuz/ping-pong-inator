#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "sam.h"

#include "can_controller.h"
#include "time.h"
#include "pwm.h"
#include "motor.h"
#include "servo.h"
#include "ir.h"
#include "timer_counter.h"
#include "solenoid.h"

// Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
// #include "uart.h"

int main() {
    SystemInit();

    servo_init();
    motor_init();

    // Disable Watchdog Timer
    WDT->WDT_MR = WDT_MR_WDDIS;


    // Initialize UART
    uart_init(F_CPU, 9600);
    printf("UART initialized\n\r");

    ir_init();
    timer_counter_init();

    // Initialize CAN
    uint32_t can_br = 0x01293165; // This was calculated by hand
    uint8_t  status = can_init_default_tx_rx_mb(can_br);
    if (status) {
        printf("CAN initialization failed\n\r");
    } else {
        printf("CAN initialization successful\n\r");
    }

    solenoid_init();

    int i = 0;
    bool prev_hit = false;
    while (1) {
        CAN0_Handler();

        // Sending messages
        // static int can_id = 0;
        // CAN_MESSAGE can_msg = {
        //   .id          = can_id++,
        //   .data_length = 6,
        //   .data        = "world!"
        // };
        // can_send(&can_msg, 0);
        // time_spinFor(msecs(1));

      
        //   servo_set_pos((i%100)/100.0f);
        // read_encoder();
        // printf("Motor pos: %d \r\n", tc_read_motor_pos());
        // printf("Motor revolutions: %d \r\n", tc_read_motor_rev());
        // tc_read_status();
        bool hit = ir_hit();
        
        if (!prev_hit && hit) {
            i++;
            printf("IR hit count: %d\r\n", i);
        }

        prev_hit = hit;
        time_spinFor(msecs(1000));
        solenoid_fire();
    }
}
