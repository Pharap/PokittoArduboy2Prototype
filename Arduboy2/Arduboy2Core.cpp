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

	//// Select the ADC input here so a delay isn't required in initRandomSeed()
	//ADMUX = RAND_SEED_IN_ADMUX;

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

	//const uint8_t oldSREG = SREG;

	//// suspend interrupts
	//cli();

	//// dissable the PLL and set prescale for 16MHz)
	//PLLCSR = _BV(PINDIV);

	//// allow reprogramming clock
	//CLKPR = _BV(CLKPCE);

	//// set clock divisor to 2 (0b0001)
	//CLKPR = 1;

	//// enable the PLL (with 16MHz prescale)
	//PLLCSR = (_BV(PLLE) | _BV(PINDIV));

	//// restore interrupts
	//SREG = oldSREG;
}
#endif

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins(void)
{
	// Arduboy2Core::bootPins(void) is intentionally unimplemented.

#ifdef ARDUBOY_10

	//// Port B INPUT_PULLUP or HIGH
	//PORTB |= (_BV(RED_LED_BIT) | _BV(GREEN_LED_BIT) | _BV(BLUE_LED_BIT) | _BV(B_BUTTON_BIT));

	//// Port B INPUT or LOW (none)

	//// Port B inputs
	//DDRB &= ~(_BV(B_BUTTON_BIT) | _BV(SPI_MISO_BIT));

	//// Port B outputs
	//DDRB |= (_BV(RED_LED_BIT) | _BV(GREEN_LED_BIT) | _BV(BLUE_LED_BIT) | _BV(SPI_MOSI_BIT) | _BV(SPI_SCK_BIT) | _BV(SPI_SS_BIT));

	//// Port C
	//// Speaker: Not set here. Controlled by audio class

	//// Port D INPUT_PULLUP or HIGH
	//PORTD |= _BV(CS_BIT);

	//// Port D INPUT or LOW
	//PORTD &= ~(_BV(RST_BIT));

	//// Port D inputs (none)

	//// Port D outputs
	//DDRD |= (_BV(RST_BIT) | _BV(CS_BIT) | _BV(DC_BIT));

	//// Port E INPUT_PULLUP or HIGH
	//PORTE |= _BV(A_BUTTON_BIT);

	//// Port E INPUT or LOW (none)

	//// Port E inputs
	//DDRE &= ~(_BV(A_BUTTON_BIT));

	//// Port E outputs (none)

	//// Port F INPUT_PULLUP or HIGH
	//PORTF |= (_BV(LEFT_BUTTON_BIT) | _BV(RIGHT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT));

	//// Port F INPUT or LOW
	//PORTF &= ~(_BV(RAND_SEED_IN_BIT));

	//// Port F inputs
	//DDRF &= ~(_BV(LEFT_BUTTON_BIT) | _BV(RIGHT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) | _BV(//RAND_SEED_IN_BIT));

	//// Port F outputs (none)

#elif defined(AB_DEVKIT)

	//// Port B INPUT_PULLUP or HIGH
	//PORTB |= (_BV(LEFT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) | _BV(BLUE_LED_BIT));

	//// Port B INPUT or LOW (none)

	//// Port B inputs
	//DDRB &= ~(_BV(LEFT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) | _BV(SPI_MISO_BIT));

	//// Port B outputs
	//DDRB |= (_BV(SPI_MOSI_BIT) | _BV(SPI_SCK_BIT) | _BV(SPI_SS_BIT) | _BV(BLUE_LED_BIT));

	//// Port C INPUT_PULLUP or HIGH
	//PORTC |= _BV(RIGHT_BUTTON_BIT);

	//// Port C INPUT or LOW (none)

	//// Port C inputs
	//DDRC &= ~(_BV(RIGHT_BUTTON_BIT));
	//// Port C outputs (none)

	//// Port D INPUT_PULLUP or HIGH
	//PORTD |= _BV(CS_BIT);

	//// Port D INPUT or LOW
	//PORTD &= ~(_BV(RST_BIT));

	//// Port D inputs (none)

	//// Port D outputs
	//DDRD |= (_BV(RST_BIT) | _BV(CS_BIT) | _BV(DC_BIT));

	//// Port E (none)

	//// Port F INPUT_PULLUP or HIGH
	//PORTF |= (_BV(A_BUTTON_BIT) | _BV(B_BUTTON_BIT));

	//// Port F INPUT or LOW
	//PORTF &= ~(_BV(RAND_SEED_IN_BIT));

	//// Port F inputs
	//DDRF &= ~(_BV(A_BUTTON_BIT) | _BV(B_BUTTON_BIT) | _BV(RAND_SEED_IN_BIT));

	//// Port F outputs (none)
	//// Speaker: Not set here. Controlled by audio class

#endif
}

void Arduboy2Core::bootOLED(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::bootOLED(void)

	// reset the display
	// reset pin should be low here. let it stay low a while
	delayShort(5);

	//// set high to come out of reset
	//bitSet(RST_PORT, RST_BIT);

	// wait a while
	delayShort(5);

	//// select the display (permanently, since nothing else is using SPI)
	//bitClear(CS_PORT, CS_BIT);

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

	//bitSet(DC_PORT, DC_BIT);
}

void Arduboy2Core::LCDCommandMode(void)
{
	// Arduboy2Core::LCDCommandMode(void) is intentionally unimplemented.

	//bitClear(DC_PORT, DC_BIT);
}

// Initialize the SPI interface for the display
void Arduboy2Core::bootSPI(void)
{
	// Arduboy2Core::bootSPI(void) is intentionally unimplemented.

	//// master, mode 0, MSB first, CPU clock / 2 (8MHz)
	//SPCR = (_BV(SPE) | _BV(MSTR));
	//SPSR = _BV(SPI2X);
}

// Write to the SPI bus (MOSI pin)
void Arduboy2Core::SPItransfer(uint8_t data)
{
	// Arduboy2Core::SPItransfer(uint8_t data) is intentionally unimplemented.

	// Intentionally unused.
	(void)data;

	//SPDR = data;

	/*
	 * The following NOP introduces a small delay that can prevent the wait
	 * loop form iterating when running at the maximum speed. This gives
	 * about 10% more speed, even if it seems counter-intuitive. At lower
	 * speeds it is unnoticed.
	 */
	//asm volatile("nop");

	//// wait
	//while((SPSR & _BV(SPIF)) == 0);
}

void Arduboy2Core::safeMode(void)
{
	if(buttonsState() != UP_BUTTON)
		return;

	digitalWriteRGB(RED_LED, RGB_ON);

	//// for Arduboy core timer 0 should remain enabled
//#ifndef ARDUBOY_CORE
	//// prevent the bootloader magic number from being overwritten by timer 0
	//// when a timer variable overlaps the magic number location
	//power_timer0_disable();
//#endif

	while(true);
}


/* Power Management */

void Arduboy2Core::idle(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::idle(void)

	//// select idle mode and enable sleeping
	//SMCR = _BV(SE);

	//sleep_cpu();

	//// disable sleeping
	//SMCR = 0;
}

void Arduboy2Core::bootPowerSaving(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::bootPowerSaving(void)

	//// disable Two Wire Interface (I2C) and the ADC
	//// All other bits will be written with 0 so will be enabled
	//PRR0 = (_BV(PRTWI) | _BV(PRADC));

	//// disable USART1
	//PRR1 |= _BV(PRUSART1);
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

	//// set display reset pin low (reset state)
	//bitClear(RST_PORT, RST_BIT);
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
	Pokitto::Arduboy2Helper::drawArduboy2Buffer(image);
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
//
// The following assembly code runs "open loop". It relies on instruction
// execution times to allow time for each byte of data to be clocked out.
// It is specifically tuned for a 16MHz CPU clock and SPI clocking at 8MHz.
void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
	Pokitto::Arduboy2Helper::drawArduboy2Buffer(image, clear);
}

void Arduboy2Core::blank(void)
{
	Pokitto::Arduboy2Helper::fillArduboyScreen(0);
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

//// RGB, all the pretty colors
//#ifdef ARDUBOY_10

	//// timer 0: Fast PWM, OC0A clear on compare / set at top
	//// We must stay in Fast PWM mode because timer 0 is used for system timing.
	//// We can't use "inverted" mode because it won't allow full shut off.
	//TCCR0A = (_BV(COM0A1) | _BV(WGM01) | _BV(WGM00));

	//OCR0A = (255 - green);

	//// timer 1: Phase correct PWM 8 bit
	//// OC1A and OC1B set on up-counting / clear on down-counting (inverted). This
	//// allows the value to be directly loaded into the OCR with common anode LED.
	//TCCR1A = (_BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM10));

	//OCR1AL = blue;
	//OCR1BL = red;

//#elif defined(AB_DEVKIT)

	//// only blue on DevKit, which is not PWM capable

	//// parameter unused
	//(void)red;

	//// parameter unused
	//(void)green;

	//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue ? RGB_ON : RGB_OFF);

//#endif
}

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
	// TODO: Investigate the best way to implement Arduboy2Core::setRGBled(uint8_t color, uint8_t val)

	(void)color;
	(void)val;

//#ifdef ARDUBOY_10

	//if(color == RED_LED)
		//OCR1BL = val;
	//else if(color == GREEN_LED)
		//OCR0A = (255 - val);
	//else if(color == BLUE_LED)
		//OCR1AL = val;

//#elif defined(AB_DEVKIT)

	//// only blue on DevKit, which is not PWM capable
	//if(color == BLUE_LED)
		//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val ? RGB_ON : RGB_OFF);

//#endif
}

void Arduboy2Core::freeRGBled(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::freeRGBled(void)

//#ifdef ARDUBOY_10
	//// clear the COM bits to return the pins to normal I/O mode
	//TCCR0A = (_BV(WGM01) | _BV(WGM00));
	//TCCR1A = _BV(WGM10);
//#endif
}

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	// TODO: Investigate the best way to implement Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)

	(void)red;
	(void)green;
	(void)blue;

//#ifdef ARDUBOY_10

	//bitWrite(RED_LED_PORT, RED_LED_BIT, red);
	//bitWrite(GREEN_LED_PORT, GREEN_LED_BIT, green);
	//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue);

//#elif defined(AB_DEVKIT)

	//// only blue on DevKit

	//// parameter unused
	//(void)red;

	//// parameter unused
	//(void)green;

	//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue);

//#endif
}

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{
	// TODO: Investigate the best way to implement Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)

	(void)color;
	(void)val;

//#ifdef ARDUBOY_10

	//if(color == RED_LED)
		//bitWrite(RED_LED_PORT, RED_LED_BIT, val);
	//else if(color == GREEN_LED)
		//bitWrite(GREEN_LED_PORT, GREEN_LED_BIT, val);
	//else if(color == BLUE_LED)
		//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val);

//#elif defined(AB_DEVKIT)

	//// only blue on DevKit
	//if(color == BLUE_LED)
		//bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val);

//#endif
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

	//cli();

	//// set bootloader magic key
	//// storing two uint8_t instead of one uint16_t saves an instruction
	////  when high and low bytes of the magic key are the same
	//(*reinterpret_cast<uint8_t *>(MAGIC_KEY_POS + 0)) = lowByte(MAGIC_KEY);
	//(*reinterpret_cast<uint8_t *>(MAGIC_KEY_POS + 1)) = highByte(MAGIC_KEY);

	//// enable watchdog timer reset, with 16ms timeout
	//wdt_reset();

	//WDTCSR = (_BV(WDCE) | _BV(WDE));
	//WDTCSR = _BV(WDE);

	while(true);
}

// Replacement main() that eliminates the USB stack code.
// Used by the ARDUBOY_NO_USB macro. This should not be called
// directly from a sketch.

void Arduboy2Core::mainNoUSB(void)
{
	// TODO: Investigate the best way to implement Arduboy2Core::mainNoUSB(void)

	//// disable USB
	//UDCON = _BV(DETACH);
	//UDIEN = 0;
	//UDINT = 0;
	//USBCON = _BV(FRZCLK);
	//UHWCON = 0;
	//power_usb_disable();

	//init();

	//// This would normally be done in the USB code that uses the TX and RX LEDs
	//TX_RX_LED_INIT;
	//TXLED0;
	//RXLED0;

	//// Set the DOWN button pin for INPUT_PULLUP
	//bitSet(DOWN_BUTTON_PORT, DOWN_BUTTON_BIT);
	//bitClear(DOWN_BUTTON_DDR, DOWN_BUTTON_BIT);

	// Delay to give time for the pin to be pulled high if it was floating
	delayShort(10);

	//// if the DOWN button is pressed
	//if(bitRead(DOWN_BUTTON_PORTIN, DOWN_BUTTON_BIT) == 0)
		//exitToBootloader();

	//// The remainder is a copy of the Arduino main() function with the
	//// USB code and other unneeded code commented out.
	//// init() was called above.
	//// The call to function initVariant() is commented out to fix compiler
	//// error: "multiple definition of 'main'".
	//// The return statement is removed since this function is type void.

	////  init();

	////  initVariant();

	////#if defined(USBCON)
	////  USBDevice.attach();
	////#endif

	//setup();

	//for(;;) {
		//loop();
		////    if(serialEventRun) serialEventRun();
	//}

	////  return 0;
}

