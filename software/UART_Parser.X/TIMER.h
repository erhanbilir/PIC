/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

#include "config.h"

void TIMER0_init(void);
void ISR_TIMER(void);

#define PRESCALER 256
#define TIMER0_2S_OVERFLOWS ( (uint16_t)(2 * _XTAL_FREQ / 4 / 256 / PRESCALER) )

#endif	/* TIMER_H */

