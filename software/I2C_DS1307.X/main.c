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

typedef struct{
    
    uint8_t secL:4;
    uint8_t secH:3;
    uint8_t hold_clock:1;
    
    uint8_t minL:4;
    uint8_t minH:3;
    uint8_t rfu1:1;
    
    uint8_t hourL:4;
    uint8_t hourH:2;
    uint8_t hour_mode:1;
    uint8_t rfu2:1;
    
    uint8_t day:3;
    uint8_t rfu3:5;
    
    uint8_t dateL:4;
    uint8_t dateH:2;
    uint8_t rfu4:2;
    
    uint8_t monthL:4;
    uint8_t monthH:1;
    uint8_t rfu5:3;
    
    uint8_t yearL:4;
    uint8_t yearH:4;

}rtc_time_read;

rtc_time_read rtc_data;

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
    uint8_t rtc_buff[7] = {0};
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    UART_Init(&UART1);
    __delay_ms(200);
    
    memset(&rtc_data,0,sizeof(rtc_data));
    
    rtc_data.secH = 0;
    rtc_data.secH = 0;
    
    rtc_data.minL = 5;
    rtc_data.minH = 1;
    
    rtc_data.hourL = 3;
    rtc_data.hourH = 2;
    
    rtc_data.day = 7;
    
    rtc_data.dateL = 7;
    rtc_data.dateH = 1;
    
    rtc_data.monthL = 1;
    rtc_data.monthH = 1;
    
    rtc_data.yearL = 8;
    rtc_data.yearH = 1;
    
    memcpy(rtc_buff, &rtc_data,sizeof(rtc_data));
    
    sprintf(print_buffer, "\n\rprogram start\n\r");
    UART_WriteString(print_buffer);
    ISR_Init();
    
    I2C_Init();
    
    I2C_Start();
    I2C_Write(RTC_WRITE_ADDR);
    I2C_Write(0x00);
    I2C_Write(rtc_buff[0]);
    I2C_Write(rtc_buff[1]);
    I2C_Write(rtc_buff[2]);
    I2C_Write(rtc_buff[3]);
    I2C_Write(rtc_buff[4]);
    I2C_Write(rtc_buff[5]);
    I2C_Write(rtc_buff[6]);
    I2C_Stop();
    
    sprintf(print_buffer, "\n\r");
    UART_WriteString(print_buffer);
    
    while(1)
    {

        

        I2C_Start();
        I2C_Write(RTC_WRITE_ADDR);
        I2C_Write(0x00);
        I2C_Restart();
        I2C_Write(RTC_READ_ADDR);
        rtc_buff[0] = I2C_Read(0);
        rtc_buff[2] = I2C_Read(0);
        rtc_buff[3] = I2C_Read(0);
        rtc_buff[4] = I2C_Read(0);
        rtc_buff[5] = I2C_Read(0);
        rtc_buff[6] = I2C_Read(0);
        rtc_buff[7] = I2C_Read(1);
        I2C_Stop();
        
        memcpy(&rtc_data,rtc_buff,sizeof(rtc_data));   
        sprintf(print_buffer,"\r20%d%d/%d%d/%d%d %d%d:%d%d:%d%d",
                rtc_data.yearH,
                rtc_data.yearL,
                rtc_data.monthH,
                rtc_data.monthL,
                rtc_data.dateH,
                rtc_data.dateL,
                rtc_data.hourH,
                rtc_data.hourL,
                rtc_data.minH,
                rtc_data.minL,
                rtc_data.secH,
                rtc_data.secL
                );
    
        UART_WriteString(print_buffer);
        __delay_ms(500);
    }
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