/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"

#include "PokittoHelper.h"

#include <Pokitto.h>

// lcdBootProgram is intentionally kept despite not being used.
const uint8_t PROGMEM lcdBootProgram[] =
{
	// boot defaults are commented out but left here in case they
	// might prove useful for reference
	//
	// Further reading: https://www.adafruit.com/datasheets/SSD1306.pdf
	//
	// Display Off
	// 0xAE,

	// Set Display Clock Divisor v = 0xF0
	// default is 0x80
	0xD5, 0xF0,

	// Set Multiplex Ratio v = 0x3F
	// 0xA8, 0x3F,

	// Set Display Offset v = 0
	// 0xD3, 0x00,

	// Set Start Line (0)
	// 0x40,

	// Charge Pump Setting v = enable (0x14)
	// default is disabled
	0x8D, 0x14,

	// Set Segment Re-map (A0) | (b0001)
	// default is (b0000)
	0xA1,

	// Set COM Output Scan Direction
	0xC8,

	// Set COM Pins v
	// 0xDA, 0x12,

	// Set Contrast v = 0xCF
	0x81, 0xCF,

	// Set Precharge = 0xF1
	0xD9, 0xF1,

	// Set VCom Detect
	// 0xDB, 0x40,

	// Entire Display ON
	// 0xA4,

	// Set normal/inverse display
	// 0xA6,

	// Display On
	0xAF,

	// set display mode = horizontal addressing mode (0x00)
	0x20, 0x00,

	// set col address range
	// 0x21, 0x00, COLUMN_ADDRESS_END,

	// set page address range
	// 0x22, 0x00, PAGE_ADDRESS_END
};


Arduboy2Core::Arduboy2Core(void)
{
}

void Arduboy2Core::boot(void)
{
// ARDUBOY_SET_CPU_8MHZ will be set by the IDE using boards.txt
#ifdef ARDUBOY_SET_CPU_8MHZ
	setCPUSpeed8MHz();
#endif

	bootPins();
	bootSPI();
	bootOLED();
	bootPowerSaving();

	// TODO: implement Arduboy2Core::boot(void)
}

#ifdef ARDUBOY_SET_CPU_8MHZ
// If we're compiling for 8MHz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz.
// We also need to readjust the PLL prescaler because the Arduino USB code
// likely will have incorrectly set it for an 8MHz hardware clock.
void Arduboy2Core::setCPUSpeed8MHz(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::setCPUSpeed8MHz(void)
}
#endif

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins(void)
{
	// Arduboy2Core::bootPins(void) is intentionally unimplemented.
}

void Arduboy2Core::bootOLED(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::bootOLED(void)

	// reset the display
	// reset pin should be low here. let it stay low a while
	delayShort(5);

	// wait a while
	delayShort(5);

	// run our customized boot-up command sequence against the
	// OLED to initialize it properly for Arduboy
	LCDCommandMode();

	for(size_t i = 0; i < sizeof(lcdBootProgram); ++i)
		SPItransfer(pgm_read_byte(&lcdBootProgram[i]));

	LCDDataMode();
}

void Arduboy2Core::LCDDataMode(void)
{
	// Arduboy2Core::LCDDataMode(void) is intentionally unimplemented.
}

void Arduboy2Core::LCDCommandMode(void)
{
	// Arduboy2Core::LCDCommandMode(void) is intentionally unimplemented.
}

// Initialize the SPI interface for the display
void Arduboy2Core::bootSPI(void)
{
	// Arduboy2Core::bootSPI(void) is intentionally unimplemented.
}

// Write to the SPI bus (MOSI pin)
void Arduboy2Core::SPItransfer(uint8_t data)
{
	// Arduboy2Core::SPItransfer(uint8_t data) is intentionally unimplemented.
	(void)data;
}

void Arduboy2Core::safeMode(void)
{
	if(buttonsState() != UP_BUTTON)
		return;

	digitalWriteRGB(RED_LED, RGB_ON);

	while(true);
}


/* Power Management */

void Arduboy2Core::idle(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::idle(void)
}

void Arduboy2Core::bootPowerSaving(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::bootPowerSaving(void)
}

// Shut down the display
void Arduboy2Core::displayOff(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::displayOff(void)

	LCDCommandMode();

	// display off
	SPItransfer(0xAE);

	// charge pump:
	SPItransfer(0x8D);

	//   disable
	SPItransfer(0x10);

	delayShort(250);
}

// Restart the display after a displayOff()
void Arduboy2Core::displayOn(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::displayOn(void)

	bootOLED();
}

uint8_t Arduboy2Core::width(void)
{
	return WIDTH;
}

uint8_t Arduboy2Core::height(void)
{
	return HEIGHT;
}


/* Drawing */

void Arduboy2Core::paint8Pixels(uint8_t pixels)
{
	// TODO: Investigate the best way to implement Arduboy2Core::paint8Pixels(uint8_t pixels)

	SPItransfer(pixels);
}

void Arduboy2Core::paintScreen(const uint8_t * image)
{
	Pokitto::Arduboy2Helper::drawBuffer(image);
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
//
// The following assembly code runs "open loop". It relies on instruction
// execution times to allow time for each byte of data to be clocked out.
// It is specifically tuned for a 16MHz CPU clock and SPI clocking at 8MHz.
void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
	Pokitto::Arduboy2Helper::drawBuffer(image, clear);
}

void Arduboy2Core::blank(void)
{
	Pokitto::Arduboy2Helper::fillDisplay(0);
}

void Arduboy2Core::sendLCDCommand(uint8_t command)
{
	// TODO: Investigate the best way to implement Arduboy2Core::sendLCDCommand(uint8_t command)

	LCDCommandMode();

	SPItransfer(command);

	LCDDataMode();
}

// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void Arduboy2Core::invert(bool inverse)
{
	// TODO: Investigate the best way to implement Arduboy2Core::invert(bool inverse)

	sendLCDCommand(inverse ? OLED_PIXELS_INVERTED : OLED_PIXELS_NORMAL);
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on)
{
	// TODO: Investigate the best way to implement Arduboy2Core::allPixelsOn(bool on)

	sendLCDCommand(on ? OLED_ALL_PIXELS_ON : OLED_PIXELS_FROM_RAM);
}

// flip the display vertically or set to normal
void Arduboy2Core::flipVertical(bool flipped)
{
	// TODO: Investigate the best way to implement Arduboy2Core::flipVertical(bool flipped)

	sendLCDCommand(flipped ? OLED_VERTICAL_FLIPPED : OLED_VERTICAL_NORMAL);
}

// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped)
{
	// TODO: Investigate the best way to implement Arduboy2Core::flipHorizontal(bool flipped)

	sendLCDCommand(flipped ? OLED_HORIZ_FLIPPED : OLED_HORIZ_NORMAL);
}

/* RGB LED */

void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
	// TODO: Investigate the best way to implement Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)

	(void)red;
	(void)green;
	(void)blue;
}

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
	// TODO: Investigate the best way to implement Arduboy2Core::setRGBled(uint8_t color, uint8_t val)

	(void)color;
	(void)val;
}

void Arduboy2Core::freeRGBled(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::freeRGBled(void)
}

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	// TODO: Investigate the best way to implement Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)

	(void)red;
	(void)green;
	(void)blue;
}

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{
	// TODO: Investigate the best way to implement Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)

	(void)color;
	(void)val;
}

/* Buttons */

uint8_t Arduboy2Core::buttonsState(void)
{
	std::uint8_t buttons = 0;
	
	if(Pokitto::heldStates[BTN_A] != 0)
		buttons |= A_BUTTON;
		
	if(Pokitto::heldStates[BTN_B] != 0)
		buttons |= B_BUTTON;
	
	if(Pokitto::heldStates[BTN_UP] != 0)
		buttons |= UP_BUTTON;
		
	if(Pokitto::heldStates[BTN_DOWN] != 0)
		buttons |= DOWN_BUTTON;
	
	if(Pokitto::heldStates[BTN_LEFT] != 0)
		buttons |= LEFT_BUTTON;
		
	if(Pokitto::heldStates[BTN_RIGHT] != 0)
		buttons |= RIGHT_BUTTON;

	return buttons;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
	Pokitto::Core::wait(ms);
}

void Arduboy2Core::exitToBootloader(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::exitToBootloader(void)

	while(true);
}

// Replacement main() that eliminates the USB stack code.
// Used by the ARDUBOY_NO_USB macro. This should not be called
// directly from a sketch.

void Arduboy2Core::mainNoUSB(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::mainNoUSB(void)

	//init();

	// Delay to give time for the pin to be pulled high if it was floating
	delayShort(10);

	//// if the DOWN button is pressed
	//if(bitRead(DOWN_BUTTON_PORTIN, DOWN_BUTTON_BIT) == 0)
		//exitToBootloader();

	// The remainder is a copy of the Arduino main() function with the
	// USB code and other unneeded code commented out.
	// init() was called above.
	// The call to function initVariant() is commented out to fix compiler
	// error: "multiple definition of 'main'".
	// The return statement is removed since this function is type void.

	//setup();

	//for(;;) {
		//loop();
	//}
}

