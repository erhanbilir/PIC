/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 29, 2025, 9:48 AM
 */
#define _XTAL_FREQ 800000
#include "config.h"

void IO_Init(void);
void PWM_Init(void);
void PWM_SetDuty(uint16_t duty);

/*!< PWM Period = [(PR2) + 1] * 4 * TOSC * (TMR2 Prescale Value)               */
/*!< PWM Duty Cycle =(CCPR1L:CCP1CON<5:4>) * TOSC * (TMR2 Prescale Value)      */
/*!< Duty cycle ratio = value / (4 * (PR2 + 1))                                */

void main(void) {
    IO_Init();
    PWM_Init();
    
    PWM_SetDuty(32);
    bool turn = false;
    while(1)
    {
        PWM_SetDuty(50);
        __delay_ms(1000);
        
        PWM_SetDuty(75);
        __delay_ms(1000);
        
        PWM_SetDuty(100);
        __delay_ms(1000);
        
        PWM_SetDuty(75);
        __delay_ms(1000);
    }
    
    return;
}

/**
 * @brief Configures I/O ports
 * RC2 pin (CCP1 output) is set as output
 */
void IO_Init(void)
{
    TRISC = 0x00;
    PORTC = 0x00;
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
    CCPR1L = duty >> 2;
}
