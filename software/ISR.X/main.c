/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 17, 2025, 10:23 AM
 */

#include "config.h"
#include <xc.h>

#define _XTAL_FREQ 64000000 

int selected_pin = 0;

void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCTUNEbits.PLLEN = 0b1;
    
    TRISBbits.RB1 = 1;
    TRISBbits.RB0 = 1;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB0 = 0;
    TRISC = 0x00;
    ANSELC = 0x00;
    LATC = 0x00;
    
    RCONbits.IPEN = 1;      /*!< Enable priorities */
    INTCONbits.GIEH = 1;    /*!< Enable high prio. interrupts */
    INTCONbits.GIEL = 1;    /*!< Enable low prio. interrupts */
    
    INTCON2bits.INTEDG1 = 1;    /*!< Rising edge triggered */
    INTCON3bits.INT1IP = 0;     /*!< Configure as low prio. */
    INTCON3bits.INT1IF = 0;     /*!< Clear the flag */
    INTCON3bits.INT1IE = 1;     /*!< Enable the interrupt */
    
    INTCON2bits.INTEDG0 = 1;    /*!< Rising edge triggered */
    INTCONbits.INT0IF = 0;      /*!< Clear the flag */
    INTCONbits.INT0E = 1;       /*!< Enable the interrupt */
    
    while(1)
    {
        LATC = 0x1F;
    }
    
    return;
}

void __interrupt(low_priority) ISR_low()
{
    LATC = 0x03;
    __delay_ms(2000);
    
    INTCON3bits.INT1IF = 0;
}

void __interrupt(high_priority) ISR_high()
{
    LATC = 0x18;
    __delay_ms(2000);
    
    INTCONbits.INT0IF = 0;
}

