/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 16, 2025, 12:19 PM
 */
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 4000000

void main(void) {
    TRISC = 0x00;
    TRISB = 0x00;
    
    PORTC = 0x00;
    PORTB = 0x00;
    
    unsigned char num = 78;
    unsigned char i, j;
    
    unsigned char digits[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x07,0x7F,0x6F};
    /*int i = 0;
    while(1)
    {
        if(i==10)
            i = 0;
        PORTC |= digits[i];
        __delay_ms(600);
        PORTC &= ~(digits[i]);
        i++;
    }*/
    for(unsigned int k = 0; k<5000;k++)
    {
        i = num / 10;
        j = num % 10;
        
        PORTB &= ~digits[i];
        PORTC &= ~digits[j];
    }
    for(unsigned int k = 0; k<5000;k++)
    {
        PORTB |= digits[i];
        PORTC |= digits[j];
               
    }

    for(unsigned int k = 0; k<5000;k++)
    {
        i = num / 10;
        j = num % 10;
        
        PORTB &= ~digits[i];
        PORTC &= ~digits[j];
    }
    for(unsigned int k = 0; k<5000;k++)
    {
        PORTB |= digits[i];
        PORTC |= digits[j];
               
    }
    for(unsigned int k = 0; k<5000;k++)
    {
        i = num / 10;
        j = num % 10;
        
        PORTB &= ~digits[i];
        PORTC &=  ~digits[j];
    }
    while(1)
    {
        PORTB &= ~digits[i];
        PORTC &= ~digits[j];
    }

    return;
}

