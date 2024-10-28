#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "sam.h"

#include "can_controller.h"
#include "time.h"
#include "pwm.h"
#include "servo.h"

// Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
// #include "uart.h"

int main() {
    SystemInit();

    pwm_init(PWM_DUTY_MIN);

    pwm_init(PWM_DUTY_MIN);

    // Disable Watchdog Timer
    WDT->WDT_MR = WDT_MR_WDDIS;


    // Initialize UART
    uart_init(F_CPU, 9600);
    printf("UART initialized\n\r");

    // Initialize CAN
    uint32_t can_br = 0x01293165; // This was calculated by hand
    uint8_t  status = can_init_default_tx_rx_mb(can_br);
    if (status) {
        printf("CAN initialization failed\n\r");
    } else {
        printf("CAN initialization successful\n\r");
    }

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

      
      servo_set_pos((i%100)/100.0f);
      i++;

      time_spinFor(msecs(20));
    }
}
