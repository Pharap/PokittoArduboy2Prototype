/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include <Arduino.h>
#include "Arduboy2Beep.h"

#ifndef AB_DEVKIT

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
	TCCR3A = 0;

	// CTC mode. Divide by 8 clock prescale
	TCCR3B = (bit(WGM32) | bit(CS31));
}

void BeepPin1::tone(uint16_t count)
{
	tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
	duration = dur;

	// set toggle on compare mode (which connects the pin)
	TCCR3A = bit(COM3A0);

	// load the count (16 bits), which determines the frequency
	OCR3A = count;
}

void BeepPin1::timer()
{
	if(duration == 0)
		return;

	--duration;

	// set normal mode (which disconnects the pin)
	if(duration == 0)
		TCCR3A = 0;
}

void BeepPin1::noTone()
{
	duration = 0;

	// set normal mode (which disconnects the pin)
	TCCR3A = 0;
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
	// normal mode. Disable PWM
	TCCR4A = 0;

	// divide by 128 clock prescale
	TCCR4B = bit(CS43);

	// normal mode
	TCCR4D = 0;

	// toggle pin at count = 0
	TC4H = 0;

	//  "
	OCR4A = 0;
}

void BeepPin2::tone(uint16_t count)
{
	tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
	duration = dur;

	// set toggle on compare mode (which connects the pin)
	TCCR4A = bit(COM4A0);

	// load the count (10 bits),
	TC4H = highByte(count);

	//  which determines the frequency
	OCR4C = lowByte(count);
}

void BeepPin2::timer()
{
	if(duration == 0)
		return;

	--duration;

	// set normal mode (which disconnects the pin)
	if(duration == 0)
		TCCR4A = 0;
}

void BeepPin2::noTone()
{
	duration = 0;

	// set normal mode (which disconnects the pin)
	TCCR4A = 0;
}


#else /* AB_DEVKIT */

// *** The pins used for the speaker on the DevKit cannot be directly
// controlled by a timer/counter. The following "dummy" functions will
// compile and operate properly but no sound will be produced

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
}

void BeepPin1::tone(uint16_t count)
{
	tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
	// parameter not used
	(void)count;

	duration = dur;
}

void BeepPin1::timer()
{
	if(duration > 0)
		--duration;
}

void BeepPin1::noTone()
{
	duration = 0;
}


uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
}

void BeepPin2::tone(uint16_t count)
{
	tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
	// parameter not used
	(void)count;

	duration = dur;
}

void BeepPin2::timer()
{
	if(duration > 0)
		--duration;
}

void BeepPin2::noTone()
{
	duration = 0;
}

#endif
