#define _XTAL_FREQ 4000000  /*!< 4 MHz crystal */

#include <xc.h>
#include <stdbool.h>

// CONFIG bits
#pragma config FOSC = HS        /*!< external high speed crystal */
#pragma config MCLRE = ON       /*!< MCLR active -> Vcc */

void main(void) {
    TRISA = 0b00000001; /*!< RA0 input  (button) */
    TRISB = 0b00000000; /*!< Port B all output (LED) */

    PORTA = 0x00;
    PORTB = 0x00;
    
    while (1) 
    {
        if(RA0 == 0)
        {
            __delay_ms(50);
            while(RA0==0);
            __delay_ms(50);
            for(int i = 0; i < 8; i++){
                PORTB |= (1 << i);
                __delay_ms(400);
            }
        }

    }
    return;
}


