#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "sam.h"

//Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
//#include "uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(F_CPU, 9600);
    printf("Hello World\n\r");

    while (1)
    {
        printf("Hello World\n\r");
    }
    
}