/*
 * File:   UART.c
 * Author: erhan
 *
 * Created on July 25, 2025, 9:35 AM
 */

#include "UART.h"
#include <string.h>

/*!< Global buffers and indices */
static __IO uint8_t rxBuffer[RX_BUFFER_SIZE];
static __IO uint8_t rxHead = 0;
static __IO uint8_t rxTail = 0;

static __IO uint8_t txBuffer[TX_BUFFER_SIZE];
static __IO uint8_t txHead = 0;
static __IO uint8_t txTail = 0;
static __IO bool txBusy = false;

static char lineBuffer[LINE_BUFFER_SIZE];
static uint8_t lineIdx = 0;

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
 * @brief Interrupt Service Routine
 * Handles UART RX and TX interrupts
 */
void ISR_UART(void)
{
    /*!< RX Interrupt */
    if(PIR1bits.RCIF)
    {
        if(RCSTAbits.OERR)
        {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
        else if(RCSTAbits.FERR)
        {
            volatile uint8_t err = RCREG;
        }
        else
        {
            UART_ReceiveHandler(RCREG);
        } 
    }
    
    /*!< TX Interrupt */
    if(PIR1bits.TXIF && PIE1bits.TXIE)
    {
        if(txHead != txTail)
        {
            TXREG = txBuffer[txHead];
            txHead = (txHead + 1) % TX_BUFFER_SIZE;
        }
        else
        {
            PIE1bits.TXIE = 0;
            txBusy = false;
        }
    }
}

/**
 * @brief Starts UART transmission (enable TX interrupt)
 */
void UART_StartTransmission(void)
{
    if(!txBusy)
    {
        txBusy = true;
        PIE1bits.TXIE = 1;
    }
}

/**
 * @brief Add single byte to TX buffer and start transmission
 */
void UART_WriteChar(uint8_t data)
{
    uint8_t nextTail = (txTail + 1) % TX_BUFFER_SIZE;
    
    if(nextTail == txHead)
    {
        TIMER0_init();

        while(nextTail == txHead && !uartTimeout);
        if(uartTimeout)
        {
            txHead = (txHead + 1) % TX_BUFFER_SIZE;
            //return;
        }
    }
    
    txBuffer[txTail] = data;
    txTail = nextTail;
    
    UART_StartTransmission();
}

/**
 * @brief Send a null-terminated string via UART
 */
void UART_WriteString(const char* str)
{
    
    while(*str)
    {
        UART_WriteChar(*str++);
    }
}

/**
 * @brief Check if there is data available in RX buffer
 * 
 * @return true if data available, false otherwise
 */
bool UART_Available(void)
{
    return (rxHead != rxTail);
}

/**
 * @brief Reads one byte from UART and handles errors
 * @param data Pointer to variable where received byte will be stored
 */
void UART_ReceiveHandler(uint8_t data)
{
    uint8_t nextHead = (rxHead + 1) % RX_BUFFER_SIZE;
    
    if(nextHead != rxTail)
    {
        rxBuffer[rxHead] = data;
        rxHead = nextHead;
    }
}

/**
 * @brief Parse string receive from UART
 * @param Pointer to buffer where received line will be stored
 */
void UART_Parse(const char* msg)
{
    char temp[LINE_BUFFER_SIZE];
    char* res = strncpy(temp,msg,LINE_BUFFER_SIZE-1);
    temp[LINE_BUFFER_SIZE-1] = '\0';
    
    char *device = strtok(temp, ":");
    char *command = strtok(NULL, "");
    
    if(device && command)
    {
        if(strcmp(device,"led")==0)
        {
            if(strcmp(command,"on")==0)
            {
                LED_On();   
            }
            else if(strcmp(command,"off")==0)
            {
                LED_Off();
            }
        }
    }
}

void LED_On(void)
{
    PORTBbits.RB6 = 1;
}

void LED_Off(void)
{
    PORTBbits.RB6 = 0;
}

/**
 * @brief Read a single byte from RX buffer
 * 
 * @return Received byte (0 if buffer empty)
 */
uint8_t UART_ReadChar(void)
{
    uint8_t data = 0;
    if(rxHead != rxTail)
    {
        data = rxBuffer[rxTail];
        rxTail = (rxTail + 1) % RX_BUFFER_SIZE;
    }
    return data;
}

/**
 * @brief Read an entire line from RX buffer
 * 
 * Reads characters until a newline ('\n' or '\r') is received.
 * 
 * @param outBuffer Pointer to buffer where received line will be stored
 * @return true if a complete line was received, false otherwise
 */
bool UART_ReadLine(char *outBuffer)
{
    while(UART_Available())
    {
        uint8_t c = UART_ReadChar();
        if(c == '\n' || c == '\r')
        {
            lineBuffer[lineIdx] = '\0';
            strcpy(outBuffer, lineBuffer);
            lineIdx = 0;
            return true;
        }
        else if(lineIdx < LINE_BUFFER_SIZE - 1)
        {
            lineBuffer[lineIdx++] = c;
        }
    }
    return false;
}