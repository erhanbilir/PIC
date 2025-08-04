/* 
 * File:   DEVICE.c
 * Author: erhan
 *
 * Created on July 25, 2025, 5:13 PM
 */

#include "DEVICE.h"
#include <string.h>
#include <stdlib.h>


void Generic_On(Device* dev)
{
    *(dev->tris) &= ~(1 << dev->pin);  // set output
    *(dev->port) |=  (1 << dev->pin);  // pin set
}

void Generic_Off(Device* dev)
{
    *(dev->tris) &= ~(1 << dev->pin);  // set output
    *(dev->port) &= ~(1 << dev->pin);  // pin reset
}

/* Turn off all devices at startup */
void Device_Init(Device* devices, uint8_t deviceCount)
{
    for(uint8_t i = 0; i < deviceCount; i++)
    {
        if(devices[i].Init)
        {
            devices[i].Init(&devices[i]);
        }
        else
        {
            *(devices[i].tris) &= ~(1 << devices[i].pin);
            *(devices[i].port) &= ~(1 << devices[i].pin);
        }
    }
}

/* Device name + command */
bool Device_Execute(Device* devices, uint8_t deviceCount, const char* name, const char* command, const char* value)
{
    for(uint8_t i = 0; i < deviceCount; i++)
    {
        if(strcmp(name, devices[i].name) == 0)
        {
            if(strcmp(command, "on") == 0)
            {
                devices[i].On(&devices[i]);
                return true;
            }
            else if(strcmp(command, "off") == 0)
            {
                devices[i].Off(&devices[i]);
                return true;
            }
            else if(strcmp(command, "setValue") == 0)
            {
                if(devices[i].SetValue && value != NULL)
                {
                    uint8_t val = (uint8_t)atoi(value);
                    devices[i].SetValue(&devices[i],val);
                }
                return true;
            }
        }
    }
    return false;
}
