# PIC Microcontroller Projects

This repository contains both software and hardware projects for Microchip PIC microcontrollers.  
It is designed to demonstrate various peripherals and techniques using the XC8 compiler and simulation environments such as Proteus.

---

## Repository Structure

### 1. **/software/**
Contains MPLAB X IDE projects showcasing different peripheral implementations:
- **7segment.X** – 7-segment display driving
- **ADC.X** – Analog-to-Digital Converter example
- **ADC_LCD.X** – ADC result displayed on an LCD
- **I2C_DS1307.X** – I²C communication with DS1307 RTC (reading seconds)
- **IOC.X** – Interrupt-on-change and Timer0 example
- **ISR.X** – Equal priority interrupt handling
- **LCD_pic.X** – Character LCD interfacing
- **PIC18F4MCU.X** – Generic device structure with port/pin configuration
- **PWM.X** – PWM with RC filter for duty cycle smoothing
- **UART.X** – UART driver with interrupt-based circular buffers
- **UART_Parser.X** – UART command parsing example
- **UART_Parser2.X** – Enhanced UART parser version
- **led_blink.X** – Simple LED blink example
- **pic18f4kx22_input_to_out.X** – Input to output forwarding example
- **timer.X** – Timer0 configuration and usage

### 2. **/hardware/**
Contains Proteus (or equivalent) simulation projects:
- **7segment.pdsprj** – 7-segment display hardware simulation
- **ADC.pdsprj** – ADC peripheral hardware testbench
- **ADC_LCD.pdsprj** – ADC + LCD integration simulation
- **I2C.pdsprj** – DS1307 I²C simulation
- **IOC.pdsprj** – Interrupt-on-change example hardware
- **PWM.pdsprj** – PWM output hardware simulation
- **UART.pdsprj** – UART peripheral simulation
- **UART_Parser.pdsprj** – UART command parser hardware
- **interrupt.pdsprj** – Button interrupt example
- **led_blink.pdsprj** – Simple LED blink simulation
- **picmcu.pdsprj** – Generic PIC MCU configuration project
- **timer.pdsprj** – Timer0 hardware simulation

---

## Features
- Software examples for PIC peripheral usage  
- Hardware simulations for visual validation  
- Clear separation between software and hardware  
- Uses structured commit messages (`feat()`, `fix()`, `refactor()`)

---

## Development Environment
- **IDE:** MPLAB X IDE (for software)
- **Compiler:** XC8
- **Simulation:** Proteus (for hardware)
- **Target MCU:** PIC16F / PIC18F series (configurable)

---

## Getting Started
1. Clone the repository:
   ```bash
   git clone https://github.com/erhanbilir/PIC.git
