/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 9 September 2018 by Pharap
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>

// Pharap: Remove <stdbool.h>, <string.h> and <math.h> includes

// Pharap: Remove <avr/interrupt.h> include
#include <avr/pgmspace.h>
#include <avr/io.h>

// Pharap: Remove "binary.h" include

// Pharap: Remove extern "C"

void yield(void);

#define HIGH 0x1
#define LOW  0x0

// Pharap: Remove INPUT, OUTPUT and INPUT_PULLUP macros to prevent name clash

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  #define DEFAULT 0
  #define EXTERNAL 1
  #define INTERNAL1V1 2
  #define INTERNAL INTERNAL1V1
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define DEFAULT 0
  #define EXTERNAL 4
  #define INTERNAL1V1 8
  #define INTERNAL INTERNAL1V1
  #define INTERNAL2V56 9
  #define INTERNAL2V56_EXTCAP 13
#else  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif

// Pharap: Remove min, max and abs macros
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void init(void);
void initVariant(void);

// Pharap: Disable atexit function to prevent name clash
//int atexit(void (*func)()) __attribute__((weak));

// Pharap: Disable pin manipulation functions
//void pinMode(uint8_t, uint8_t);
//void digitalWrite(uint8_t, uint8_t);
//int digitalRead(uint8_t);
//int analogRead(uint8_t);
//void analogReference(uint8_t mode);
//void analogWrite(uint8_t, int);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);

// Pharap: Remove pulseIn and pulseInLong functions

// Pharap: Remove shiftOut and shiftIn functions

// Pharap: Remove attachInterrupt and detachInterrupt functions

void setup(void);
void loop(void);

// Pharap: Remove analogInPinToBit macro

// Pharap: Remove port_to_mode_PGM, port_to_input_PGM, port_to_output_PGM arrays

// Pharap: Remove digital_pin_to_port_PGM, digital_pin_to_bit_PGM, digital_pin_to_bit_mask_PGM, digital_pin_to_timer_PGM arrays

// Pharap: Remove digitalPinToPort, digitalPinToBitMask, digitalPinToTimer macros
// Pharap: Remove analogInPinToBit macro
// Pharap: Remove portOutputRegister, portInputRegister, portModeRegister macros

// Pharap: Remove NOT_A_PIN and NOT_A_PORT macros

// Pharap: Remove NOT_AN_INTERRUPT macro

// Pharap: Remove PA, PB, PC, PD, PE, PF, PG, PH, PJ, PK, PL macros

// Pharap: Remove TIMER macros

// Pharap: Remove ifdef __cplusplus

// Pharap: Remove "WCharacter.h" include

#include "WString.h"

// Pharap: Remove "HardwareSerial.h" and "USBAPI.h" includes

#if defined(HAVE_HWSERIAL0) && defined(HAVE_CDCSERIAL)
#error "Targets with both UART0 and CDC serial not supported"
#endif

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

// Pharap: Remove pulseIn and pulseInLong functions

// Pharap: Disable tone and noTone functions
//void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
//void noTone(uint8_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

#endif

// Pharap: Remove "pins_arduino.h" include
