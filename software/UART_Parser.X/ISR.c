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
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
}
