/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 29, 2025, 9:48 AM
 */
#define _XTAL_FREQ 800000
#include "config.h"
#include "ISR.h"
#include "UART.h"

void IO_Init(void);
void PWM_Init(void);
void PWM_SetDuty(uint16_t duty);
void __interrupt() ISR(void);
float Calculate_Degree(uint16_t duty);


/*!< PWM Period = [(PR2) + 1] * 4 * TOSC * (TMR2 Prescale Value)               */
/*!< PWM Duty Cycle =(CCPR1L:CCP1CON<5:4>) * TOSC * (TMR2 Prescale Value)      */
/*!< Duty cycle ratio = value / (4 * (PR2 + 1))                                */

char command[1];


void main(void) {
    ISR_Init();
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 20}; // 2400 baud
    UART_Init(&UART1);
    char msg[20] = "program started\n\r";
    UART_WriteString(msg);
    IO_Init();
    PWM_Init();
    uint16_t duty = 50;
    float f;
    PWM_SetDuty(duty);
    while(1)
    {
        if(UART_ReadLine(command))
        {
            if(command[0] == '-')
            {
                if(duty > 50)
                    duty--;
                UART_WriteString("Servo set ");
                f = Calculate_Degree(duty);
                UART_WriteFloat(f,2);
                UART_WriteString(" degree\n\r");
            }
            if(command[0] == '+')
            {
                if(duty < 100)
                    duty++;
                UART_WriteString("Servo set ");
                f = Calculate_Degree(duty);
                UART_WriteFloat(f,2);
                UART_WriteString(" degree\n\r");
            }
            PWM_SetDuty(duty);
        }
        /*PWM_SetDuty(50);
        __delay_ms(1000);
        
        PWM_SetDuty(75);
        __delay_ms(1000);
        
        PWM_SetDuty(100);
        __delay_ms(1000);
        
        PWM_SetDuty(75);
        __delay_ms(1000);*/
    }
    
    return;
}

/**
 * @brief Configures I/O ports
 * RC2 pin (CCP1 output) is set as output
 */
void IO_Init(void)
{
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;
}

/**
 * @brief Initializes the PWM module
 * CCP1 is set to PWM mode and Timer2 is enabled
 */
void PWM_Init(void)
{
    TMR2 = 0;               /*!< Clear Timer2            */
    CCPR1L = 0;
    PR2 = 249;               /*!< Set PWM period          */
    T2CON = 0b00000111;     /*!< Timer ON, Prescaler = 16 */
    CCP1CON = 0b00001100;   /*!< CCP1 in PWM mode        */
}

/**
 * @brief Sets the PWM duty cycle
 * @param duty 10-bit duty cycle value
 */
void PWM_SetDuty(uint16_t duty)
{
    /*!< Lower 2 bits */
    CCP1CONbits.CCP1Y = (duty >> 0) & 0x01;  // Bit 0 
    CCP1CONbits.CCP1X = (duty >> 1) & 0x01;  // Bit 1
    
    /*!< Higher 8 bits */
    CCPR1L = (uint8_t)(duty >> 2);
}

float Calculate_Degree(uint16_t duty)
{
    return (float)(((duty-50) * (3.6)) + 0.005);
}

void __interrupt() ISR(void)
{
    if (PIR1bits.RCIF)
    {
        ISR_UART();
    }
    if (PIR1bits.TXIF)
    {
        ISR_UART();
    }
}