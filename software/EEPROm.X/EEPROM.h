/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "config.h"
#include "ISR.h"

uint8_t EEPROM_Read(uint8_t addr);
void EEPROM_Write(uint8_t addr, uint8_t data);

#endif	/* EEPROM_H */

