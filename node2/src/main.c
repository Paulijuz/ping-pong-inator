#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "sam.h"

#include "can_controller.h"
#include "time.h"
#include "pwm.h"

//Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
//#include "uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
    uart_init(F_CPU, 9600);

    uint32_t can_br = 0x01293165;
    uint8_t  status = can_init_def_tx_rx_mb(can_br);
    if (status) {
      printf("CAN initialization failed\n\r");
    } else {
      printf("CAN initialization successful\n\r");
    }

    uint8_t i = 0;
    while (1) {
      CAN0_Handler();

      // Sending messages
      // CAN_MESSAGE can_msg = {
      //   .id          = i++,
      //   .data_length = 6,
      //   .data        = "world!"
      // };
      // can_send(&can_msg, 0);
      // time_spinFor(msecs(1));
    }
}
