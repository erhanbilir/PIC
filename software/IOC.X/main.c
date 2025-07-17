/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 17, 2025, 2:01 PM
 */

#include "config.h"
#include <xc.h>

#define _XTAL_FREQ 64000000

volatile char previous_PORTB_RB4 = 0;
volatile char previous_PORTB_RB5 = 0;

void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCTUNEbits.PLLEN = 0b1;
    
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
    
    
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 1;
    ANSELBbits.ANSB4 = 0;
    ANSELBbits.ANSB5 = 0;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    
    IOCBbits.IOCB4 = 1;
    IOCBbits.IOCB5 = 1;
    PORTB; /*!< Update hidden IOC register */
    INTCON2bits.RBIP = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    while(1)
    {
        LATDbits.LATD2 = 1;
        LATDbits.LATD3 = 1;
    }
    
    return;
}

void __interrupt(high_priority) ISR_high()
{
    if(INTCONbits.RBIF == 1 && INTCONbits.RBIE == 1)
    {
        /*!< Delay for debouncing */
        __delay_ms(50);
        
        /*!< Read the current PORTB */
        char current_PORTB = PORTB;
        
        /*!< Extract the RB5 and RB4 pins from current PORTB*/
        char current_PORTB_RB5 = (current_PORTB & 0b00100000) >> 5;
        char current_PORTB_RB4 = (current_PORTB & 0b00010000) >> 4;
        
        /*!< If a pin went from low to high state*/
        if (previous_PORTB_RB5 == 0 && current_PORTB_RB5 == 1)
        {
            LATDbits.LATD2 = 1;
            LATDbits.LATD3 = 0;
        }
        else if (previous_PORTB_RB4 == 0 && current_PORTB_RB4 == 1)
        {
            LATDbits.LATD2 = 0;
            LATDbits.LATD3 = 1;
        }
        
        __delay_ms(1000);

        
        /*!< Read the current PORTB and Extract the RB5 and RB4 pins from current PORTB */
        current_PORTB = PORTB;
        previous_PORTB_RB5 = (current_PORTB & 0b00100000) >> 5;
        previous_PORTB_RB4 = (current_PORTB & 0b00010000) >> 4;
        
        

        INTCONbits.RBIF = 0;
    }

}

void __interrupt(low_priority) ISR_low()
{
}