/*
 * File:   main.c
 * Author: erhan
 *
 * Created on August 8, 2025, 6:02 PM
 */


#include "config.h"
#include "UART.h"
#include "EEPROM.h"
#include "ISR.h"

#define _XTAL_FREQ 16000000

void __interrupt() ISR(void);

uint8_t run = 1;
volatile uint8_t led_flash = 0x00;
char print_buffer[32] = {0};
volatile uint8_t data = 0;
bool got_data_bool = false;

void main(void) {
    TRISB = 0x00;
    PORTB = 0x00;
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    UART_Init(&UART1);
    
    __delay_ms(20);
    ISR_Init();
    
    __delay_ms(20);
    
    led_flash = EEPROM_Read(0x00);
    PORTB = ((1u) << led_flash);
    
    sprintf(print_buffer,"\n\rNumber of Blinks: %c, %d\n\r",led_flash,led_flash);
    UART_WriteString(print_buffer);
    
    while(1)
    {
        if(got_data_bool)
        {
            data = UART_ReadByte();
            if(data != '\0' && data != '\r' && data != '\n')
            {
                data = data - '0';
                EEPROM_Write(0x00,data);
                sprintf(print_buffer, "\rSave to EEPROM: %d",data);
                got_data_bool = false;
                UART_WriteString(print_buffer);
            }
            led_flash = EEPROM_Read(0x00);
            PORTB = ((1u) << led_flash);
        }
    }
    
   return;
}

void __interrupt() ISR(void)
{
    if (PIR1bits.RCIF || PIR1bits.TXIF)
    {
        if(PIR1bits.RCIF)
            got_data_bool = true;
        ISR_UART();
    }
}