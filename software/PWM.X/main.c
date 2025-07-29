/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 29, 2025, 9:48 AM
 */
#define _XTAL_FREQ 8000000
#include "config.h"

void IO_Init(void);
void PWM_Init(void);
void PWM_SetDuty(uint16_t duty);

/*!< PWM Period = [(PR2) + 1] ? 4 ? TOSC ? (TMR2 Prescale Value)               */
/*!<          ==> 100 ? 4 ? 125 ns ? 4 = 200 ?s                                */
/*!< PWM Duty Cycle =(CCPR1L:CCP1CON<5:4>) ? TOSC ? (TMR2 Prescale Value)      */
/*!<          ==> [value] ? 125 ns ? 4                                         */
/*!< Duty cycle ratio = value / (4 * (PR2 + 1))                               */
/*!< value = 100  =>  25% duty => ~1.25V average (assuming 5V supply)         */
/*!< value = 399  => ~99.75% duty => ~5V average (assuming 5V supply)         */

void main(void) {
    IO_Init();
    PWM_Init();
    
    uint16_t duty = 0;
    while(1)
    {
        PWM_SetDuty(duty);
        __delay_ms(20);
        duty++;
        if(duty >= 399)
        {
            duty = 0;
        }
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
    CCP1CON = 0b00001111;   /*!< CCP1 in PWM mode        */
    T2CON = 0b00000101;     /*!< Timer ON, Prescaler = 4 */
    PR2 = 99;               /*!< Set PWM period          */
    TMR2 = 0;               /*!< Clear Timer2            */
}

/**
 * @brief Sets the PWM duty cycle
 * @param duty 10-bit duty cycle value (0 - 399)
 */
void PWM_SetDuty(uint16_t duty)
{
    /*!< Lower 2 bits */
    CCP1CONbits.CCP1Y = (duty & 0x1);
    CCP1CONbits.CCP1X = (duty & 0x2);
    
    /*!< Higher 8 bits */
    CCPR1L = duty >> 2;
}
