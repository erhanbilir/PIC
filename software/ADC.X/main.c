    /* 
 * File:   main.c
 * Author: erhan
 *
 * Created on July 28, 2025, 3:42 PM
 */
#define _XTAL_FREQ 8000000
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

unsigned int ADC_Read(unsigned char channel);

void main(void) {
    TRISA = 0xFF;
    TRISB = 0x00;
    TRISC = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    ADCON0 = 0b10000001;
    ADCON1 = 0b10000000;
    
    unsigned int x;
    while(1)
    {
        x = ADC_Read(0);
        PORTB = x;
        PORTC = x >> 8;
    }
    
    return;
}

unsigned int ADC_Read(unsigned char channel)
{
    if (channel > 7)
        return 0;
    
    ADCON0 &= 0b11000101;
    ADCON0 |= channel << 3;
    __delay_ms(2);
    GO_DONE = 1;
    while(GO_DONE);
    return (unsigned int)( (ADRESH << 8) + ADRESL );
}