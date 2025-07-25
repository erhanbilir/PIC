/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 25, 2025, 9:35 AM
 */

#include "config.h"
#include "ISR.h"
#include "UART.h"

void __interrupt() ISR(void);
__IO bool uartTimeout = false;

void main(void) {
    UART_Init_TypeDef UART1;
    UART1.sync = 0;
    UART1.brgh = 1;
    UART1.gen_reg = 25;
    TRISBbits.TRISB6 = 0;
    PORTBbits.RB6 = 0;

    UART_Init(&UART1);
    ISR_Init();
    
    UART_WriteString("UART Ready\r\n");
    
    char line[64];
    
    while(1)
    {
        if(UART_ReadLine(line))
        {
            UART_Parse(line);
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

