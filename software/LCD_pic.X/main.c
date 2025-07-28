/*
 * File:   main.c
 * Author: erhan
 *
 * Created on July 18, 2025, 11:20 AM
 */

#include "config.h"
#include "LCD.h"
#include <xc.h>

void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCTUNEbits.PLLEN = 0b1;
    
    TRISBbits.RB0 = 1;
    ANSELBbits.ANSB0 = 0;
    RCONbits.IPEN = 1;
    
    INTCONbits.GIEH = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    
    
    LCD_init();
    
    char customChar[] = {
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
      };
    LCD_add_character(customChar,6);
    LCD_cursor_set(1,1);
    LCD_write_string("Tuba ");
   
    LCD_write_char(6);
    LCD_write_string(" Erhan"); 

        
    while(1)
    {
        LCD_shift_left();
        __delay_ms(250);

    }
    
    return;
}

void __interrupt(high_priority) ISR_high()
{
    if(INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1)
    {
        INTCONbits.INT0IF = 0;
        LCD_shift_reset();
    }
}