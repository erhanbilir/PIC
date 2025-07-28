/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 28, 2025, 4:37 PM
 */

#define _XTAL_FREQ 8000000
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include "LCD.h"

unsigned int ADC_Read(uint8_t channel);
void IO_Init(void);

void main(void) {
    IO_Init();
    LCD_init();
    LCD_clear();
    
    float volt;
    unsigned int analog;
    char row1[20];
    char row2[20];
    
    while(1)
    {
        analog = ADC_Read(0);
        volt = analog * 5 / 1023;
        sprintf(row1,"Analog: %d",analog);
        LCD_cursor_set(1,1);
        LCD_write_string(row1);
        sprintf(row2,"Volt: %.2f",volt);
        LCD_cursor_set(2,1);
        LCD_write_string(row2);
        __delay_ms(100);
        LCD_clear();
    }
    
    return;
}

unsigned int ADC_Read(uint8_t channel)
{
    if(channel > 7)
        return 0;
    
    ADCON0 &= 11000101;
    ADCON0 |= channel << 3;
    __delay_ms(2);
    GO_DONE = 1;
    while(GO_DONE);
    return (unsigned int)( (ADRESH << 8) + ADRESL);
}

void IO_Init(void)
{
    TRISA = 0xFF;
    TRISD = 0x00;
    PORTD = 0x00;
    ADCON0 = 0b10000001;
    ADCON1 = 0b10000000;
}


