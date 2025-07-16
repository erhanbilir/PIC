/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 16, 2025, 4:18 PM
 */

#include "config.h"
#include <xc.h>

void main(void) 
{
    /*<! Configure the oscillator block (64MHz) */
    OSCCONbits.IRCF = 0b111;
    OSCTUNEbits.PLLEN = 0b1;
    
    TRISC = 0x0F;
    ANSELC = 0x00;
    while(1)
    {
        LATCbits.LATC4 = PORTCbits.RC0;
        LATCbits.LATC5 = PORTCbits.RC1;
        LATCbits.LATC6 = PORTCbits.RC2;
        LATCbits.LATC7 = PORTCbits.RC3;

    }
            
    return;
}
