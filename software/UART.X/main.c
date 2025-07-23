/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 23, 2025, 11:00 AM
 */

#include "config.h"
#include <xc.h>
#include "stdint.h"
#include "stdbool.h"

typedef struct
{
    uint8_t gen_reg;
    uint8_t sync;
    uint8_t brgh;
}UART_Init_TypeDef;

void __interrupt() ISR(void);
void ISR_Init(void);
void UART_Init(UART_Init_TypeDef *UART_ConfigStruct);
void UART_Send(uint8_t data);
void UART_Receive();

uint8_t rx_data;

void main(void) {
    UART_Init_TypeDef UART1;
    UART1.sync = 0;
    UART1.brgh = 1;
    UART1.gen_reg = 25;
    
    UART_Init(&UART1);
    
    while(1)
    {
        __delay_ms(200);
        UART_Send('A');

    }
    return;
}
void ISR_Init(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TXIE = 0;
    PIE1bits.RCIE = 1;
}

void UART_Init(UART_Init_TypeDef *UART_ConfigStruct)
{
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
 
    SPBRG = UART_ConfigStruct->gen_reg;
    
    TXSTAbits.SYNC = UART_ConfigStruct->sync;
    TXSTAbits.BRGH = UART_ConfigStruct->brgh;
    
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}

void __interrupt() ISR(void)
{
    if(PIR1bits.RCIF)
    {
        rx_data = RCREG;
    }
    if(PIR1bits.TXIF && PIE1bits.TXIE)
    {
    }
}

void UART_Send(uint8_t data)
{
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

