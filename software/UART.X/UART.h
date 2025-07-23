/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include "config.h"
#include <stdint.h>
#include <stdbool.h>

#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32
#define LINE_BUFFER_SIZE 32

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
void ISR_UART(void);
void UART_Init(UART_Init_TypeDef *UART_ConfigStruct);
void UART_StartTransmission(void);
void UART_WriteChar(uint8_t data);
void UART_WriteString(const char *str);
void UART_ReceiveHandler(uint8_t data);
bool UART_Available(void);
uint8_t UART_ReadChar(void);
bool UART_ReadLine(char *outBuffer);

#endif	/* UART_H */

