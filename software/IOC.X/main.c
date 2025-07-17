/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 17, 2025, 2:01 PM
 */

#include "config.h"
#include <xc.h>

#define _XTAL_FREQ 64000000

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
        LATDbits.LATD2 = 1;
        LATDbits.LATD3 = 0;
        __delay_ms(500);
        PORTB;
        NOP();

        INTCONbits.RBIF = 0;
    }

}

void __interrupt(low_priority) ISR_low()
{
}