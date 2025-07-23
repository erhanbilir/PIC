/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 23, 2025, 11:00 AM
 */

#include "config.h"
#include "ISR.h"
#include "UART.h"

void __interrupt() ISR(void);

void main(void) {
    UART_Init_TypeDef UART1;
    UART1.sync = 0;
    UART1.brgh = 1;
    UART1.gen_reg = 25;

    UART_Init(&UART1);
    ISR_Init();
    
    UART_WriteString("UART Ready\r\n");
    
    char line[32];
    
    while(1)
    {
        if(UART_ReadLine(line))
        {
            UART_WriteString("Received: ");
            UART_WriteString(line);
            UART_WriteString("\r\n");
        }
    }
}

void __interrupt() ISR(void)
{
    ISR_UART();
}

