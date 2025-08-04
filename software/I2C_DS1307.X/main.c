/*
 * File:   main.c
 * Author: erhan
 *
 * Created on August 1, 2025, 5:08 PM
 */

#define _XTAL_FREQ 4000000
#include "config.h"
#include "UART.h"
#include "ISR.h"

#define RTC_WRITE_ADDR (0xD0)
#define RTC_READ_ADDR  (0xD1)

uint8_t uart_data = 0;
char print_buffer[32] = {0};

void __interrupt() ISR(void);

extern void ISR_Init(void);


void I2C_Is_Idle(void);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Restart(void);
void I2C_Stop(void);
uint8_t I2C_Read(uint8_t ack);
void I2C_Write(uint8_t data);
void I2C_Ack(void);
void I2C_Nack(void);
void I2C_Wait(void);

void main(void) {
    __delay_ms(200);
    
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    UART_Init(&UART1);
    __delay_ms(200);

    sprintf(print_buffer, "\n\rprogram start\n\r");
    UART_WriteString(print_buffer);
    ISR_Init();
    I2C_Init();
    uint8_t sec = 0;
    while(1)
    {
        I2C_Start();
        I2C_Write(RTC_WRITE_ADDR);
        I2C_Write(0x00);
        I2C_Restart();
        I2C_Write(RTC_READ_ADDR);
        sec = I2C_Read(1);
        I2C_Stop();
        sprintf(print_buffer, "sec: 0x%02x\n\r",sec);
    
        UART_WriteString(print_buffer);
        __delay_ms(1000);
    }


    


    while(1);
    
    return;
}

void I2C_Is_Idle(void)
{
    while( (SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void I2C_Init(void)
{
    TRISC3 = 1;
    TRISC4 = 1;
    
    SSPSTATbits.SMP = 1;
    SSPSTATbits.CKE = 0;
    SSPCONbits.SSPM = 0x08;
    SSPADD = ((_XTAL_FREQ)/(4*100000))-1;
    SSPCONbits.SSPEN = 1;
}

void I2C_Start(void)
{
    I2C_Is_Idle();
    SSPCON2bits.SEN = 1;
    while(SEN);
}

void I2C_Restart(void)
{
    I2C_Is_Idle();
    SSPCON2bits.RSEN = 1;
    while(RSEN);
}

void I2C_Stop(void)
{
    I2C_Is_Idle();
    SSPCON2bits.PEN = 1;
    while(PEN);
}

uint8_t I2C_Read(uint8_t ack)
{
    I2C_Is_Idle();
    SSPCON2bits.RCEN = 1;
    while(SSPSTATbits.BF != 1);
    uint8_t receive = SSPBUF;
    if(!ack)
        I2C_Ack();
    else
        I2C_Nack();
    return receive;
}

void I2C_Write(uint8_t data)
{
    I2C_Is_Idle();
    SSPBUF = data;
    while(SSPSTATbits.BF != 0);
    while(SSPCON2bits.ACKSTAT != 0);
}

void I2C_Ack(void)
{
    ACKDT = 0;
    ACKEN = 1;
}

void I2C_Nack(void)
{
    ACKDT = 1;
    ACKEN = 1;
    while(ACKEN);
}

/*void I2C_Wait(void)
{
    while(!SSPIF);
    SSPIF = 0;
}*/

void __interrupt() ISR(void)
{
    if (PIR1bits.RCIF || PIR1bits.TXIF)
    {
        ISR_UART();
    }
}