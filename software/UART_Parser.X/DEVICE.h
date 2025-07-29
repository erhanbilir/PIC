/* 
 * File:   
 * Author: erhan
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Device
{
    const char* name;                    // device name
    volatile unsigned char* tris;        // TRIS register pointer
    volatile unsigned char* port;        // PORT register pointer
    volatile unsigned char* lat;         // LAT register pointer (only PIC32)
    uint8_t pin;                         // pin number
    
    void (*Init)(struct Device* dev);
    void (*On)(struct Device* dev);      // turn on function
    void (*Off)(struct Device* dev);     // turn off function
    void (*Update)(struct Device* dev);
    void (*SetValue)(struct Device* dev, uint8_t value);
} Device;

/* Function prototypes */
void Generic_On(Device* dev);
void Generic_Off(Device* dev);
void Device_Init(Device* devices, uint8_t deviceCount);
bool Device_Execute(Device* devices, uint8_t deviceCount, const char* name, const char* command, const char* value);

#endif


