/*
  main.cpp - Main loop for Arduino sketches
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
  Modified 10 September 2018 by Pharap
  Modified 18 April 2019 by Pharap
*/

// Pharap: Change <Arduino.h> to "Arduino.h"
#include "Arduino.h"

// Pharap: Disabled atexit to avoid name clash with mbed
// Declared weak in Arduino.h to allow user redefinitions.
//int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

// Pharap: Disable setupUSB
//void setupUSB() __attribute__((weak));
//void setupUSB() { }

// Pharap: Add <Pokitto.h> include
#include <Pokitto.h>

int main()
{
	using Pokitto::Core;
	using Pokitto::Display;

	init();

	initVariant();
	
	// Pharap: Remove USB code

	// Pharap: Add Pokitto::Core::begin() call
	Core::begin();
	Display::persistence = static_cast<std::uint8_t>(~0);
	
	setup();

	// Pharap: Rewrite loop calling code
	while(Core::isRunning())
		loop();
	
	// Pharap: Remove serial event code

	return 0;
}
