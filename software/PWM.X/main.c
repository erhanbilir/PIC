/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 29, 2025, 9:48 AM
 */
#define _XTAL_FREQ 8000000
#include "config.h"

void IO_Init(void);
void PWM_Init(uint8_t duty);

void main(void) {
    IO_Init();
    PWM_Init(100);
    
    while(1);
    
    return;
}

void IO_Init(void)
{
    TRISC = 0x00;
    PORTC = 0x00;
}

void PWM_Init(uint8_t duty)
{
    CCP1CON = 0b00001111;
    T2CON = 0b00000101;
    PR2 = 99;
    TMR2 = 0;
    
    CCP1CONbits.CCP1Y = (duty & 0x1);
    CCP1CONbits.CCP1X = (duty & 0x2);
    CCPR1L = duty >> 2;
}
