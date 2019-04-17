/**
 * @file Arduboy2Audio.cpp
 * \brief
 * The Arduboy2Audio class for speaker and sound control.
 */

#include "Arduboy2.h"
#include "Arduboy2Audio.h"

bool Arduboy2Audio::audio_enabled = false;

void Arduboy2Audio::on()
{
	// TODO: Implement Arduboy2Audio::on()

	audio_enabled = true;
}

void Arduboy2Audio::off()
{
	audio_enabled = false;

	// TODO: Implement Arduboy2Audio::off()
}

void Arduboy2Audio::toggle()
{
	if(audio_enabled)
		off();
	else
		on();
}

void Arduboy2Audio::saveOnOff()
{
	// TODO: Implement Arduboy2Audio::saveOnOff()

	//EEPROM.update(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void Arduboy2Audio::begin()
{
	// TODO: Implement Arduboy2Audio::begin()

	//if(EEPROM.read(EEPROM_AUDIO_ON_OFF) != 0)
	if(true)
		on();
	else
		off();
}

bool Arduboy2Audio::enabled()
{
	return audio_enabled;
}
