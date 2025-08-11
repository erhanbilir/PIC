/*
 * File:   ISR.c
 * Author: erhan
 *
 * Created on July 25, 2025, 9:35 AM
 */

#include "ISR.h"

/** 
 * @brief Enables global and peripheral interrupts and enables UART RX interrupt
 */
void ISR_Init(void)
{
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    PIE1bits.CCP1IE = 1;
    PIE1bits.TMR1IE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void ISR_Enable(void)
{
    INTCONbits.GIE = 1;
}

void ISR_Disable(void)
{
    INTCONbits.GIE = 0;
}