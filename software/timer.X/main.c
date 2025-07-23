/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 21, 2025, 2:51 PM
 */
#include "config.h"
#include <xc.h>

#define _XTAL_FREQ 8000000

void IO_init(void);
void TIMER_init(void);
void __interrupt() ISR(void);

volatile unsigned char counter = 0;

void main(void) {
    
    IO_init();
    TIMER_init();
    
    while(1);

    return;
}

/**
 * @brief Initializes I/O pins.
 *        Sets the RB6 pin as output and initializes it to LOW.
 */
void IO_init(void)
{
    TRISBbits.TRISB6 = 0;       // Set RB6 as output
    PORTBbits.RB6 = 0;          // Initialize RB6 to LOW
}

/**
 * @brief Configures TIMER0 settings.
 *        Uses the internal clock source and sets the prescaler to 256.
 *        Enables Timer0 and global interrupts.
 *        Initializes the TMR0 counter to 0 at the beginning.
 */
void TIMER_init(void)
{
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
 *         Toggles the RB6 pin after 30 Timer0 overflows.
 */
void __interrupt() ISR(void)
{
    if(INTCONbits.TMR0IF)
    {
        TMR0 = 0;               // Reset Timer0 to 0
        INTCONbits.TMR0IF = 0;  // Clear Timer0 interrupt flag
        counter++;              // Increment overflow counter
        
        if(counter>=30)         // Toggle RB6 every 30 overflows
        {
            counter=0;
            PORTBbits.RB6 ^= 1; // Toggle RB6 pin
        }
    }
}

