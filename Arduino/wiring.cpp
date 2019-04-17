/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 9 September 2018 by Pharap
*/

// Pharap: Remove "wiring_private.h" include
// Pharap: Add "Arduino.h" include
#include "Arduino.h"

// Pharap: Define DISABLEAVRMIN
#define DISABLEAVRMIN

// Pharap: Add <Pokitto.h> include
#include <Pokitto.h>

// Pharap: Add <mbed_wait_api.h> include
#include <mbed_wait_api.h>

// Pharap: Add <limits> include
#include <limits>

// Pharap: Remove MICROSECONDS_PER_TIMER0_OVERFLOW, MILLIS_INC, FRACT_INC and FRACT_MAX

// Pharap: Remove timer0_overflow_count, timer0_millis and timer0_fract

// Pharap: Remove timer0 overflow interrupt

// Pharap: Change millis implementation
unsigned long millis()
{
	return Pokitto::Core::getTime();
}

// Pharap: Change micros implementation
unsigned long micros()
{
	return millis() * 1000;
}

// Pharap: Change delay implementation
void delay(unsigned long ms)
{
	constexpr auto maxWait = std::numeric_limits<int>::max();

	unsigned long remaining = ms;

	while(remaining > maxWait)
	{
		wait_ms(static_cast<int>(maxWait));
		remaining -= maxWait;
	}

	if(remaining > 0)
		wait_ms(static_cast<int>(remaining));
}

// Pharap: Change delayMicroseconds implementation
void delayMicroseconds(unsigned int us)
{
	constexpr auto maxWait = std::numeric_limits<int>::max();
	
	unsigned long remaining = us;

	while(remaining > maxWait)
	{
		wait_us(static_cast<int>(maxWait));
		remaining -= maxWait;
	}

	if(remaining > 0)
		wait_us(static_cast<int>(remaining));
}

// Pharap: Disable init functionality
void init()
{
	// TODO: Decide how to implement init()
}
