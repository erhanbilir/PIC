
/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 25, 2025, 9:35 AM
 */

#include "config.h"
#include "ISR.h"
#include "UART.h"
#include "DEVICE.h"

void __interrupt() ISR(void);
__IO bool uartTimeout = false;


Device devices[] =
{
    {
        .name = "led",
        .tris = &TRISB,
        .port = &PORTB,
        .lat  = NULL,
        .pin  = 6,
        .On   = Generic_On,
        .Off  = Generic_Off
    },
    {
        .name = "fan",
        .tris = &TRISB,
        .port = &PORTB,
        .lat  = NULL,
        .pin  = 7,
        .On   = Generic_On,
        .Off  = Generic_Off
    }
};
const uint8_t deviceCount = sizeof(devices)/sizeof(devices[0]);


void main(void) {
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    TRISBbits.TRISB6 = 0;
    PORTBbits.RB6 = 0;

    UART_Init(&UART1);
    ISR_Init();
    
    Device_Init(devices, deviceCount);
    
    UART_WriteString("UART Ready\r\n");
    
    char line[64];
    
    while(1)
    {
        if(UART_ReadLine(line))
        {
            if(!Device_Execute(devices, deviceCount, strtok(line,":"), strtok(NULL,"")))
            {
                UART_WriteString("Unknown device or command\r\n");
            }
            UART_WriteString("Received: ");
            UART_WriteString(line);
            UART_WriteString("\r\n");
        }
    }
}

void __interrupt() ISR(void)
{
    if (PIR1bits.RCIF || PIR1bits.TXIF)
    {
        ISR_UART();
    }
    if(INTCONbits.TMR0IE)
    {
        ISR_TIMER();
    }
}
