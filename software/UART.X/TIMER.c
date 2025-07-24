/*
 * File:   timer.c
 * Author: erhan
 *
 * Created on July 24, 2025, 4:00 PM
 */


#include "config.h"

__IO unsigned char counter = 0;

/**
 * @brief Configures TIMER0 settings.
 *        Uses the internal clock source and sets the prescaler to 256.
 *        Enables Timer0 and global interrupts.
 *        Initializes the TMR0 counter to 0 at the beginning.
 */
void TIMER0_init(void)
{
    uartTimeout = false;
    /*!< OPTION_REG register configuration */
    OPTION_REGbits.T0CS = 0;    // Timer0 clock source: internal (Fosc/4)
    OPTION_REGbits.T0SE = 0;    // Increment on low-to-high edge (not used here)
    OPTION_REGbits.PSA  = 0;    // Prescaler is assigned to Timer0
    OPTION_REGbits.PS0  = 1;    // Prescaler = 256 (PS2:PS0 = 111)
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS2  = 1;
    
    /*!< INTCON register TMR0 configuration */
    INTCONbits.TMR0IF   = 0;    // Clear Timer0 interrupt flag
    INTCONbits.TMR0IE   = 1;    // Enable Timer0 interrupt
    INTCONbits.GIE      = 1;    // Enable global interrupts
    TMR0 = 0;                   // Initialize Timer0 to 0
}

/**
 * @brief  Interrupt Service Routine (ISR) for handling Timer0 overflow.
 */
void ISR_TIMER(void)
{
    if(INTCONbits.TMR0IF)
    {
        TMR0 = 0;               // Reset Timer0 to 0
        INTCONbits.TMR0IF = 0;  // Clear Timer0 interrupt flag
        counter++;              // Increment overflow counter
        
        if(counter>=60)         
        {
            counter=0;
            INTCONbits.TMR0IE   = 0;    // Disable Timer0 interrupt
            uartTimeout = true;
        }
    }
}

