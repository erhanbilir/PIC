
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
void Fan_On(Device* dev);
void Fan_Off(Device* dev);
void Set_Value(Device* dev, uint8_t value);
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
        .Off  = Generic_Off,
        .SetValue = NULL
    },
    {
        .name = "fan",
        .tris = &TRISB,
        .port = &PORTB,
        .lat  = NULL,
        .pin  = 7,
        .On   = Fan_On,
        .Off  = Fan_Off,
        .SetValue = Set_Value

    }
};
const uint8_t deviceCount = sizeof(devices)/sizeof(devices[0]);


void main(void) {
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    UART_Init(&UART1);
    Device_Init(devices, deviceCount);
    ISR_Init();
    
    Device_Init(devices, deviceCount);
    
    UART_WriteString("UART Ready\r\n");
    
    char line[32];
    char lineCopy[32];

  
    while(1)
    {
        if(UART_ReadLine(line))
        {
            strcpy(lineCopy, line);

            char *name = strtok(line, ":");
            char *command = strtok(NULL, ":");
            char *value = strtok(NULL, "");

            if(!Device_Execute(devices, deviceCount, name, command, value))
            {
                UART_WriteString("Unknown device or command\r\n");
            }
            UART_WriteString("Received: ");
            UART_WriteString(lineCopy);
            UART_WriteString("\r\n");
        }
    }
}

void Fan_On(Device* dev)
{
    *(dev->tris) &= ~(1 << dev->pin);
    *(dev->port) |=  (1 << dev->pin);
    UART_WriteString("Fan started\r\n");
}

void Fan_Off(Device* dev)
{
    *(dev->tris) &= ~(1 << dev->pin);
    *(dev->port) &= ~(1 << dev->pin);
    UART_WriteString("Fan stopped\r\n");
}

void Set_Value(Device* dev, uint8_t value)
{
    UART_WriteString(dev->name);
    UART_WriteString(" set to ");
    char buf[5];
    sprintf(buf, "%d", value);
    UART_WriteString(buf);
    UART_WriteString("\r\n");
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
