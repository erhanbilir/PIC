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

/**
 * @brief Structure for UART configuration parameters
 */
typedef struct
{
    uint8_t gen_reg;
    uint8_t sync;
    uint8_t brgh;
}UART_Init_TypeDef;

/*!< Function prototypes */
void __interrupt() ISR(void);
void ISR_Init(void);
void UART_Init(UART_Init_TypeDef *UART_ConfigStruct);
void UART_Send(uint8_t *data);
void UART_Receive(uint8_t *data);

uint8_t rx_data;

void main(void) {
    UART_Init_TypeDef UART1;
    UART1.sync = 0;
    UART1.brgh = 1;
    UART1.gen_reg = 25;
    
    UART_Init(&UART1);
    ISR_Init();
    
    while(1);
    
    return;
}

/** 
 * @brief Enables global and peripheral interrupts and enables UART RX interrupt
 */
void ISR_Init(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TXIE = 0;
    PIE1bits.RCIE = 1;
}

/**
 * @brief Configures the UART hardware using provided settings
 * @param UART_ConfigStruct Pointer to configuration structure
 */
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

/**
 * @brief Main Interrupt Service Routine
 * Handles UART RX interrupts and echoes received data.
 */
void __interrupt() ISR(void)
{
    if(PIR1bits.RCIF)
    {
        UART_Receive(&rx_data);
        UART_Send(&rx_data);
    }
}

/**
 * @brief Sends a single byte over UART
 * @param data Pointer to byte to be transmitted
 */
void UART_Send(uint8_t *data)
{
    while(!TXSTAbits.TRMT);
    TXREG = *data;
}

/**
 * @brief Reads one byte from UART and handles errors
 * @param data Pointer to variable where received byte will be stored
 */
void UART_Receive(uint8_t *data)
{
    if(RCSTAbits.OERR)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    else if(RCSTAbits.FERR)
    {
        uint8_t err = RCREG;
    }
    else
    {
        *data = RCREG;
    }
}
