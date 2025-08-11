/*
 * File:   EEPROM.c
 * Author: erhan
 *
 * Created on August 11, 2025, 5:40 PM
 */
#include "EEPROM.h"

uint8_t EEPROM_Read(uint8_t addr)
{
    STATUSbits.RP1 = 1;
    STATUSbits.RP0 = 0;
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    while(EECON1bits.RD);
    return EEDATA;
}

void EEPROM_Write(uint8_t addr, uint8_t data)
{
    STATUSbits.RP1 = 1;
    STATUSbits.RP0 = 0;
    EEADR = addr;       
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    ISR_Disable();
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while(EECON1bits.WR);
    ISR_Enable();
    EECON1bits.WREN = 0;
}