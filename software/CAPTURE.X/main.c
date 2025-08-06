/*
 * File:   main.c
 * Author: erhan
 *
 * Created on August 4, 2025, 4:03 PM
 */

#include "config.h"
#include "UART.h"
#include "ISR.h"
#define _XTAL_FREQ 4000000
#define TIME_PER_OVERFLOW   (131070u) /*!< Time per Timer1 overflow in microseconds */
#define TIME_PER_TICK       (2u)     /*!< Time per Timer1 tick in microseconds (prescaler included) */

typedef struct {
    uint8_t ccpL; /*!< CCP1 low byte value */
    uint8_t ccpH; /*!< CCP1 high byte value */
} cap_uint16;

volatile cap_uint16 capture_value; /*!< Stores captured Timer1 value */
uint16_t ccp1value = 0;             /*!< Reserved variable (not used currently) */
uint32_t overflow_tmr1 = 0;         /*!< Timer1 overflow counter */
uint32_t cap_overflow_tmr1 = 0;     /*!< Overflow count saved at capture moment */
bool new_time_bool = false;         /*!< Indicates if a new capture is ready */

void __interrupt() ISR(void);

char print_buffer[32] = {0};

uint32_t time_after_capture(uint32_t overflow, cap_uint16 *capture_now);

/**
 * @brief Calculates total elapsed time based on Timer1 overflows and captured value
 * 
 * @param overflow Number of Timer1 overflows counted before capture
 * @param capture_now Pointer to the captured Timer1 register values (high & low bytes)
 * @return Elapsed time in microseconds (µs)
 */
uint32_t time_after_capture(uint32_t overflow, cap_uint16 *capture_now)
{
    uint16_t cap_value = ((uint16_t)capture_now->ccpH << 8) | capture_now->ccpL;
    return ((overflow * TIME_PER_OVERFLOW) + (cap_value * TIME_PER_TICK));
}

void main(void) {
    uint32_t time_ccp = 0;
    TRISCbits.TRISC2 = 1;
    
    UART_Init_TypeDef UART1 = {.sync = 0, .brgh = 1, .gen_reg = 25};
    UART_Init(&UART1);
    
    /*!< Configure Timer1 (prescaler 1:2, internal clock) */
    TMR1 = 0;
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1SYNC = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
    
    /*!< Configure CCP1 in capture mode (every rising edge) */
    CCP1CONbits.CCP1M3 = 0;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 1;
    
    ISR_Init();
    sprintf(print_buffer, "\n\rstart program\n\r");
    UART_WriteString(print_buffer);
    
    while(1)
    {
        if(new_time_bool)
        {
            time_ccp = time_after_capture(cap_overflow_tmr1, &capture_value);
            sprintf(print_buffer, "\rtime press %10lums",time_ccp/1000);
            UART_WriteString(print_buffer);
            new_time_bool = false;
        }
    }
    
    return;
}

/**
 * @brief Interrupt Service Routine for UART, CCP1 Capture, and Timer1 Overflow
 */
void __interrupt() ISR(void)
{
    if(PIR1bits.RCIF || PIR1bits.TXIF)
    {
        ISR_UART();
    }
    if(PIR1bits.CCP1IF)
    {
        PIR1bits.CCP1IF = 0;
        cap_overflow_tmr1 = overflow_tmr1;  /*!< Save overflow count at capture */
        capture_value.ccpL = CCPR1L;        /*!< Save low byte */
        capture_value.ccpH = CCPR1H;        /*!< Save high byte */
        overflow_tmr1 = 0;                  /*!< Reset overflow counter */
        new_time_bool = true;               /*!< Mark new capture available */
    }
    if(PIR1bits.TMR1IF)
    {
        overflow_tmr1++;                    /*!< Increment overflow counter */
        PIR1bits.TMR1IF = 0;
    }
}
