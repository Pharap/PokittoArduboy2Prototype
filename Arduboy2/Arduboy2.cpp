/**
 * @file Arduboy2.cpp
 * \brief
 * The Arduboy2Base and Arduboy2 classes and support objects and definitions.
 */

#include "Arduboy2.h"
#include "ab_logo.cpp"
#include "glcdfont.cpp"

#include <algorithm>
#include <cstdlib>

#include <Pokitto.h>

//========================================
//========== class Arduboy2Base ==========
//========================================

uint8_t Arduboy2Base::sBuffer[];

Arduboy2Base::Arduboy2Base()
{
	currentButtonState = 0;
	previousButtonState = 0;

	// frame management
	setFrameDuration(16);
	frameCount = 0;
	justRendered = false;
}

// functions called here should be public so users can create their
// own init functions if they need different behavior than `begin`
// provides by default
void Arduboy2Base::begin()
{
	// raw hardware
	boot();

	// blank the display (sBuffer is global, so cleared automatically)
	display();

	// light the RGB LED and screen if UP button is being held.
	flashlight();

	// check for and handle buttons held during start up for system control
	systemButtons();

	audio.begin();

	bootLogo();
	// alternative logo functions. Work the same as bootLogo() but may reduce
	// memory size if the sketch uses the same bitmap drawing function
	//  bootLogoCompressed();
	//  bootLogoSpritesSelfMasked();
	//  bootLogoSpritesOverwrite();
	//  bootLogoSpritesBSelfMasked();
	//  bootLogoSpritesBOverwrite();

	// wait for all buttons to be released
	waitNoButtons();
}

void Arduboy2Base::flashlight()
{
	if(!pressed(UP_BUTTON))
		return;

	// TODO: Substitute for allPixelsOn
	// smaller than allPixelsOn()
	sendLCDCommand(OLED_ALL_PIXELS_ON);
	digitalWriteRGB(RGB_ON, RGB_ON, RGB_ON);

	while(true)
		idle();
}

void Arduboy2Base::systemButtons()
{
	while(pressed(B_BUTTON))
	{
		// turn on blue LED
		digitalWriteRGB(BLUE_LED, RGB_ON);

		sysCtrlSound(UP_BUTTON | B_BUTTON, GREEN_LED, 0xFF);
		sysCtrlSound(DOWN_BUTTON | B_BUTTON, RED_LED, 0x00);

		delayShort(200);
	}

	// turn off blue LED
	digitalWriteRGB(BLUE_LED, RGB_OFF);
}

void Arduboy2Base::sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal)
{
	if(pressed(buttons))
	{
		// turn off blue LED
		digitalWriteRGB(BLUE_LED, RGB_OFF);
		delayShort(200);

		// turn on "acknowledge" LED
		digitalWriteRGB(led, RGB_ON);
		EEPROM.update(EEPROM_AUDIO_ON_OFF, eeVal);
		delayShort(500);

		// turn off "acknowledge" LED
		digitalWriteRGB(led, RGB_OFF);

		// Wait for button release
		while(pressed(buttons));
	}
}

void Arduboy2Base::bootLogo()
{
	bootLogoShell(drawLogoBitmap);
}

void Arduboy2Base::drawLogoBitmap(int16_t y)
{
	drawBitmap(20, y, arduboy_logo, 88, 16);
}

void Arduboy2Base::bootLogoCompressed()
{
	bootLogoShell(drawLogoCompressed);
}

void Arduboy2Base::drawLogoCompressed(int16_t y)
{
	drawCompressed(20, y, arduboy_logo_compressed);
}

void Arduboy2Base::bootLogoSpritesSelfMasked()
{
	bootLogoShell(drawLogoSpritesSelfMasked);
}

void Arduboy2Base::drawLogoSpritesSelfMasked(int16_t y)
{
	Sprites::drawSelfMasked(20, y, arduboy_logo_sprite, 0);
}

void Arduboy2Base::bootLogoSpritesOverwrite()
{
	bootLogoShell(drawLogoSpritesOverwrite);
}

void Arduboy2Base::drawLogoSpritesOverwrite(int16_t y)
{
	Sprites::drawOverwrite(20, y, arduboy_logo_sprite, 0);
}

void Arduboy2Base::bootLogoSpritesBSelfMasked()
{
	bootLogoShell(drawLogoSpritesBSelfMasked);
}

void Arduboy2Base::drawLogoSpritesBSelfMasked(int16_t y)
{
	SpritesB::drawSelfMasked(20, y, arduboy_logo_sprite, 0);
}

void Arduboy2Base::bootLogoSpritesBOverwrite()
{
	bootLogoShell(drawLogoSpritesBOverwrite);
}

void Arduboy2Base::drawLogoSpritesBOverwrite(int16_t y)
{
	SpritesB::drawOverwrite(20, y, arduboy_logo_sprite, 0);
}

// bootLogoText() should be kept in sync with bootLogoShell()
// if changes are made to one, equivalent changes should be made to the other
void Arduboy2Base::bootLogoShell(void (*drawLogo)(int16_t))
{
	const bool showLEDs = readShowBootLogoLEDsFlag();

	if(!readShowBootLogoFlag())
		return;

	if(showLEDs)
		digitalWriteRGB(RED_LED, RGB_ON);

	for(int16_t y = -16; y <= 24; ++y)
	{
		if(pressed(RIGHT_BUTTON))
		{
			// all LEDs off
			digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
			return;
		}

		if(showLEDs && (y == 4))
		{
			// red LED off
			digitalWriteRGB(RED_LED, RGB_OFF);
			// green LED on
			digitalWriteRGB(GREEN_LED, RGB_ON);
		}

		// Using display(CLEAR_BUFFER) instead of clear() may save code space.
		// The extra time it takes to repaint the previous logo isn't an issue.
		display(CLEAR_BUFFER);

		// call the function that actually draws the logo
		(*drawLogo)(y);

		display();
		delayShort(15);
	}

	if(showLEDs)
	{
		// green LED off
		digitalWriteRGB(GREEN_LED, RGB_OFF);

		// blue LED on
		digitalWriteRGB(BLUE_LED, RGB_ON);
	}

	delayShort(400);
	digitalWriteRGB(BLUE_LED, RGB_OFF);

	bootLogoExtra();
}

// Virtual function overridden by derived class
void Arduboy2Base::bootLogoExtra()
{
}

// wait for all buttons to be released
void Arduboy2Base::waitNoButtons()
{
	do
	{
		// simple button debounce
		delayShort(50);
	}
	while(buttonsState() != 0);
}

/* Frame management */

void Arduboy2Base::setFrameRate(uint8_t rate)
{
	eachFrameMillis = (1000 / rate);
}

void Arduboy2Base::setFrameDuration(uint8_t duration)
{
	eachFrameMillis = duration;
}

bool Arduboy2Base::everyXFrames(uint8_t frames)
{
	return ((frameCount % frames) == 0);
}

bool Arduboy2Base::nextFrame()
{
	const uint8_t now = static_cast<uint8_t>(millis());
	const uint8_t frameDurationMs = (now - thisFrameStart);

	if(justRendered)
	{
		lastFrameDurationMs = frameDurationMs;
		justRendered = false;

		return false;
	}

	if(frameDurationMs < eachFrameMillis)
	{
		// Only idle if at least a full millisecond remains, since idle() may
		// sleep the processor until the next millisecond timer interrupt.
		if((frameDurationMs + 1) < eachFrameMillis)
			idle();

		return false;
	}

	// pre-render
	justRendered = true;
	thisFrameStart = now;
	++frameCount;

	return true;
}

bool Arduboy2Base::nextFrameDEV()
{
	if(!nextFrame())
		return false;

	// TODO: Implement TXLED1 and TXLED0

	//if(lastFrameDurationMs > eachFrameMillis)
		//TXLED1;
	//else
		//TXLED0;

	return true;
}

int Arduboy2Base::cpuLoad()
{
	return ((lastFrameDurationMs * 100) / eachFrameMillis);
}

void Arduboy2Base::initRandomSeed()
{
	srandom(Pokitto::Core::getTime());
}

/* Graphics */

void Arduboy2Base::clear()
{
	fillScreen(BLACK);
}


// Used by drawPixel to help with left bitshifting since AVR has no
// multiple bit shift instruction.  We can bit shift from a lookup table
// in flash faster than we can calculate the bit shifts on the CPU.
const uint8_t bitshift_left[] PROGMEM =
{
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7)
};

void Arduboy2Base::drawPixel(int16_t x, int16_t y, uint8_t color)
{
	#ifdef PIXEL_SAFE_MODE
	if((x < 0) || (x > (WIDTH - 1)) || (y < 0) || (y > (HEIGHT - 1)))
		return;
	#endif

	const std::size_t row = (y / 8);
	const std::size_t bitIndex = (y % 8);
	const std::size_t bufferIndex = ((row * WIDTH) + x);
	const std::size_t bit = (1 << bitIndex);

	if(color != 0)
		// Set bit in screen buffer
		sBuffer[bufferIndex] |= bit;
	else
		// Clear bit in screen buffer
		sBuffer[bufferIndex] &= ~bit;
}

uint8_t Arduboy2Base::getPixel(uint8_t x, uint8_t y)
{
	const uint8_t row = (y / 8);
	const uint8_t bit_position = (y % 8);

	return ((sBuffer[(row * WIDTH) + x] & _BV(bit_position)) >> bit_position);
}

void Arduboy2Base::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	int16_t f = (1 - r);
	int16_t ddF_x = 1;
	int16_t ddF_y = (-2 * r);
	int16_t x = 0;
	int16_t y = r;

	while(x < y)
	{
		if(f >= 0)
		{
			--y;
			ddF_y += 2;
			f += ddF_y;
		}

		++x;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void Arduboy2Base::drawCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t corners, uint8_t color)
{
	int16_t f = (1 - r);
	int16_t ddF_x = 1;
	int16_t ddF_y = (-2 * r);
	int16_t x = 0;
	int16_t y = r;

	while(x < y)
	{
		if(f >= 0)
		{
			--y;
			ddF_y += 2;
			f += ddF_y;
		}

		++x;
		ddF_x += 2;
		f += ddF_x;

		// lower right
		if((corners & 0x4) != 0)
		{
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}

		// upper right
		if((corners & 0x2) != 0)
		{
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}

		// lower left
		if((corners & 0x8) != 0)
		{
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}

		// upper left
		if((corners & 0x1) != 0)
		{
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void Arduboy2Base::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
	drawFastVLine(x0, y0 - r, (2 * r) + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

void Arduboy2Base::fillCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t sides, int16_t delta, uint8_t color)
{
	int16_t f = (1 - r);
	int16_t ddF_x = 1;
	int16_t ddF_y = (-2 * r);
	int16_t x = 0;
	int16_t y = r;

	while(x < y)
	{
		if(f >= 0)
		{
			--y;
			ddF_y += 2;
			f += ddF_y;
		}

		++x;
		ddF_x += 2;
		f += ddF_x;

		int16_t yy = y * 2;
		int16_t xx = x * 2;

		// right side
		if((sides & 0x1) != 0)
		{
			drawFastVLine(x0 + x, y0 - y, yy + (1 + delta), color);
			drawFastVLine(x0 + y, y0 - x, xx + (1 + delta), color);
		}

		// left side
		if((sides & 0x2) != 0)
		{
			drawFastVLine(x0 - x, y0 - y, yy + (1 + delta), color);
			drawFastVLine(x0 - y, y0 - x, xx + (1 + delta), color);
		}
	}
}

void Arduboy2Base::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
	// bresenham's algorithm - thx wikpedia
	const bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

	if(steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if(x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	const int8_t ystep = (y0 < y1) ? 1 : -1;
	const int16_t dx = (x1 - x0);
	const int16_t dy = std::abs(y1 - y0);

	int16_t err = (dx / 2);

	for(int16_t x = x0, y = y0; x <= x1; ++x)
	{
		if(steep)
			drawPixel(y, x, color);
		else
			drawPixel(x, y, color);

		err -= dy;

		if(err < 0)
		{
			y += ystep;
			err += dx;
		}
	}
}

void Arduboy2Base::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, (y + h) - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine((x + w) - 1, y, h, color);
}

void Arduboy2Base::drawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color)
{
	const int16_t end = (y + h);

	for(int16_t a = std::max<int16_t>(0, y); a < std::min<int16_t>(end, HEIGHT); ++a)
		drawPixel(x, a, color);
}

void Arduboy2Base::drawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color)
{
	// Do y bounds checks
	if((y < 0) || (y >= HEIGHT))
		return;

	// last x point + 1
	int16_t xEnd = (x + w);

	// Check if the entire line is not on the display
	if((xEnd <= 0) || (x >= WIDTH))
		return;

	// Don't start before the left edge
	if(x < 0)
		x = 0;

	// Don't end past the right edge
	if(xEnd > WIDTH)
		xEnd = WIDTH;

	// buffer pointer plus row offset + x offset
	uint8_t * pBuf = &sBuffer[(((y / 8) * WIDTH) + x)];

	// pixel mask
	const uint8_t mask = (1 << (y & 7));

	// calculate actual width (even if unchanged)
	uint8_t width = (xEnd - x);

	switch (color)
	{
		case WHITE:
			while(width > 0)
			{
				*pBuf |= mask;
				++pBuf;
				--width;
			}
			break;

		case BLACK:
			while(width > 0)
			{
				*pBuf &= ~mask;
				++pBuf;
				--width;
			}
			break;
	}
}

void Arduboy2Base::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
	// stupidest version - update in subclasses if desired!
	for(int16_t i = x; i < (x + w); ++i)
		drawFastVLine(i, y, h, color);
}

void Arduboy2Base::fillScreen(uint8_t color)
{	
	const std::uint8_t value = (color == 0) ? 0x00 : 0xFF;
	
	for(std::size_t i = 0; i < ((WIDTH * HEIGHT) / 8); ++i)
		sBuffer[i] = value;
}

void Arduboy2Base::drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
	const uint8_t diameter = r * 2;

	// smarter version

	// Top
	drawFastHLine(x + r, y, w - diameter, color);

	// Bottom
	drawFastHLine(x + r, (y + h) - 1, w - diameter, color);

	// Left
	drawFastVLine(x, y + r, h - diameter, color);

	// Right
	drawFastVLine((x + w) - 1, y + r, h - diameter, color);

	// draw four corners
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper((x + w) - (r + 1), y + r, r, 2, color);
	drawCircleHelper((x + w) - (r + 1), (y + h) - (r + 1), r, 4, color);
	drawCircleHelper(x + r, (y + h) - (r + 1), r, 8, color);
}

void Arduboy2Base::fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
	const uint8_t diameter = r * 2;

	// smarter version
	fillRect(x + r, y, w - diameter, h, color);

	// draw four corners
	fillCircleHelper((x + w) - (r + 1), y + r, r, 1, h - (diameter + 1), color);
	fillCircleHelper(x + r, y + r, r, 2, h - (diameter + 1), color);
}

void Arduboy2Base::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

void Arduboy2Base::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if(y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}

	if(y1 > y2)
	{
		swap(y2, y1);
		swap(x2, x1);
	}

	if(y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}

	if(y0 == y2)
	// Handle awkward all-on-same-line case as its own thing
	{
		int16_t a = x0;
		int16_t b = x0;

		if(x1 < a)
			a = x1;
		else if(x1 > b)
			b = x1;

		if(x2 < a)
			a = x2;
		else if(x2 > b)
			b = x2;

		drawFastHLine(a, y0, (b - a) + 1, color);
		return;
	}

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).

	const int16_t dx02 = (x2 - x0);
	const int16_t dy02 = (y2 - y0);

	// Include y1 scanline or skip it
	const int16_t last = (y1 == y2) ? (y1 + 1) : y1;

	{
		const int16_t dx01 = (x1 - x0);
		const int16_t dy01 = (y1 - y0);

		int16_t sa = 0;
		int16_t sb = 0;

		for(int16_t y = y0; y < last; ++y)
		{
			int16_t a = (x0 + (sa / dy01));
			int16_t b = (x0 + (sb / dy02));

			sa += dx01;
			sb += dx02;

			if(a > b)
				swap(a,b);

			drawFastHLine(a, y, (b - a) + 1, color);
		}
	}

	{
		const int16_t dx12 = (x2 - x1);
		const int16_t dy12 = (y2 - y1);

		// For lower part of triangle, find scanline crossings for segments
		// 0-2 and 1-2.  This loop is skipped if y1=y2.
		int16_t sa = (dx12 * (last - y1));
		int16_t sb = (dx02 * (last - y0));

		for(int16_t y = last; y <= y2; ++y)
		{
			int16_t a = (x1 + (sa / dy12));
			int16_t b = (x0 + (sb / dy02));

			sa += dx12;
			sb += dx02;

			if(a > b)
				swap(a,b);

			drawFastHLine(a, y, (b - a) + 1, color);
		}
	}
}

void Arduboy2Base::drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t w, uint8_t h, uint8_t color)
{
	// no need to draw at all if we're offscreen
	if(((x + w) < 0) || (x > (WIDTH - 1)) || ((y + h) < 0) || (y > (HEIGHT - 1)))
		return;

	const int tempYOffset = (std::abs(y) % 8);
	const int tempSRow = (y / 8);

	const int yOffset = (y < 0) ? (8 - tempYOffset) : tempYOffset;
	const int sRow = (y < 0) ? (tempSRow - 1) : tempSRow;

	const int tempRows = (h / 8);
	const int rows = ((h % 8) != 0) ?  (tempRows + 1) : tempRows;

	for(int a = 0; a < rows; ++a)
	{
		const int bRow = sRow + a;

		if(bRow > ((HEIGHT / 8) - 1))
			break;

		if(bRow <= -2)
			continue;

		for(int iCol = 0; iCol < w; ++iCol)
		{
			if((iCol + x) > (WIDTH - 1))
				break;

			if((iCol + x) < 0)
				continue;

			if(bRow >= 0)
			{
				if(color == WHITE)
					sBuffer[(bRow * WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a * w) + iCol) << yOffset;
				else if(color == BLACK)
					sBuffer[(bRow * WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap + (a * w) + iCol) << yOffset);
				else
					sBuffer[(bRow * WIDTH) + x + iCol] ^= pgm_read_byte(bitmap + (a * w) + iCol) << yOffset;
			}

			if((yOffset != 0) && (bRow < ((HEIGHT / 8) - 1)) && (bRow > -2))
			{
				if(color == WHITE)
					sBuffer[((bRow + 1) * WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a * w) + iCol) >> (8 - yOffset);
				else if(color == BLACK)
					sBuffer[((bRow + 1) * WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap + (a * w) + iCol) >> (8 - yOffset));
				else
					sBuffer[((bRow + 1) * WIDTH) + x + iCol] ^= pgm_read_byte(bitmap + (a * w) + iCol) >> (8 - yOffset);
			}
		}
	}
}


void Arduboy2Base::drawSlowXYBitmap(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t w, uint8_t h, uint8_t color)
{
	// no need to draw at all of we're offscreen
	if(((x + w) < 0) || (x > (WIDTH - 1)) || ((y + h) < 0) || (y > (HEIGHT - 1)))
		return;

	const int16_t byteWidth = ((w + 7) / 8);

	for(int16_t yi = 0; yi < h; ++yi)
		for(int16_t xi = 0; xi < w; ++xi)
		{
			const int16_t index = ((yi * byteWidth) + (xi / 8));
			const int16_t mask = (128 >> (xi % 8));

			if((pgm_read_byte(&bitmap[index]) & mask) != 0)
				drawPixel(x + xi, y + yi, color);
		}
}


// Helper for drawCompressed()
struct BitStreamReader
{
	const uint8_t * source;
	uint16_t sourceIndex;
	uint8_t bitBuffer;
	uint8_t byteBuffer;

	BitStreamReader(const uint8_t * source)
		: source(source), sourceIndex(), bitBuffer(), byteBuffer()
	{
	}

	uint16_t readBits(uint16_t bitCount)
	{
		uint16_t result = 0;

		for(uint16_t i = 0; i < bitCount; ++i)
		{
			if(this->bitBuffer == 0)
			{
				this->bitBuffer = 0x1;
				this->byteBuffer = pgm_read_byte(&this->source[this->sourceIndex]);
				++this->sourceIndex;
			}

			if((this->byteBuffer & this->bitBuffer) != 0)
				result |= (1 << i);
				// result |= bitshift_left[i];

			this->bitBuffer <<= 1;
		}

		return result;
	}
};

void Arduboy2Base::drawCompressed(int16_t sx, int16_t sy, const uint8_t * bitmap, uint8_t color)
{
	// set up decompress state
	BitStreamReader cs = BitStreamReader(bitmap);

	// read header
	const int width = static_cast<int>(cs.readBits(8)) + 1;
	const int height = static_cast<int>(cs.readBits(8)) + 1;

	// no need to draw at all if we're offscreen
	if(((sx + width) < 0) || (sx > (WIDTH - 1)) || ((sy + height) < 0) || (sy > (HEIGHT - 1)))
		return;

	// sy = sy - (frame * height);
	const int tempYOffset = (std::abs(sy) % 8);
	const int tempStartRow = (sy / 8);
	const int yOffset = (sy < 0) ? (8 - tempYOffset) : tempYOffset;
	const int startRow = (sy < 0) ? (tempStartRow - 1) : tempStartRow;

	const int tempRows = (height / 8);
	const int rows = ((height % 8) != 0) ? (tempRows + 1) : tempRows;

	uint8_t byte = 0x00;
	uint8_t bit = 0x01;

	// + (frame*rows))
	int rowOffset = 0;
	int columnOffset = 0;

	// starting colour
	uint8_t spanColour = static_cast<uint8_t>(cs.readBits(1));
	while(rowOffset < rows)
	{
		uint16_t bitLength = 1;

		while(cs.readBits(1) == 0)
			bitLength += 2;

		// span length
		const uint16_t len = (cs.readBits(bitLength) + 1);

		// draw the span
		for(uint16_t i = 0; i < len; ++i)
		{
			if(spanColour != 0)
				byte |= bit;

			bit <<= 1;

			// reached end of byte
			if(bit != 0)
				continue;

			// draw
			const int bRow = (startRow + rowOffset);

			//if(byte) // possible optimisation
			if((bRow <= ((HEIGHT / 8) - 1)) && (bRow > -2) && ((columnOffset + sx) <= (WIDTH - 1)) && ((columnOffset + sx) >= 0))
			{
				const int16_t offset = ((bRow * WIDTH) + sx + columnOffset);

				if(bRow >= 0)
				{
					const int16_t index = offset;
					const uint8_t value = (byte << yOffset);

					if(color != 0)
						sBuffer[index] |= value;
					else
						sBuffer[index] &= ~value;
				}

				if((yOffset != 0) && (bRow < ((HEIGHT / 8) - 1)))
				{
					const int16_t index = (offset + WIDTH);
					const uint8_t value = (byte >> (8 - yOffset));

					if(color != 0)
						sBuffer[index] |= value;
					else
						sBuffer[index] &= ~value;
				}
			}

			// iterate
			++columnOffset;
			if(columnOffset >= width)
			{
				columnOffset = 0;
				++rowOffset;
			}

			// reset byte
			byte = 0x00;
			bit = 0x01;
		}

		// toggle colour bit (bit 0) for next span
		spanColour ^= 0x01;
	}
}

void Arduboy2Base::display()
{
	paintScreen(sBuffer);
}

void Arduboy2Base::display(bool clear)
{
	paintScreen(sBuffer, clear);
}

uint8_t * Arduboy2Base::getBuffer()
{
	return sBuffer;
}

bool Arduboy2Base::pressed(uint8_t buttons)
{
	return ((buttonsState() & buttons) == buttons);
}

bool Arduboy2Base::notPressed(uint8_t buttons)
{
	return ((buttonsState() & buttons) == 0);
}

void Arduboy2Base::pollButtons()
{
	previousButtonState = currentButtonState;
	currentButtonState = buttonsState();
}

bool Arduboy2Base::justPressed(uint8_t button)
{
	return (((previousButtonState & button) == 0) && ((currentButtonState & button) != 0));
}

bool Arduboy2Base::justReleased(uint8_t button)
{
	return (((previousButtonState & button) != 0) && ((currentButtonState & button) == 0));
}

bool Arduboy2Base::collide(Point point, Rect rect)
{
	return ((point.x >= rect.x) && (point.x < (rect.x + rect.width)) && (point.y >= rect.y) && (point.y < (rect.y + rect.height)));
}

bool Arduboy2Base::collide(Rect rect1, Rect rect2)
{
	return !((rect2.x >= (rect1.x + rect1.width)) || ((rect2.x + rect2.width) <= rect1.x) || (rect2.y >= (rect1.y + rect1.height)) || ((rect2.y + rect2.height) <= rect1.y));
}

uint16_t Arduboy2Base::readUnitID()
{
	return ((static_cast<uint16_t>(EEPROM.read(EEPROM_UNIT_ID + 0)) << 0) | ((static_cast<uint16_t>(EEPROM.read(EEPROM_UNIT_ID + 1))) << 8));
}

void Arduboy2Base::writeUnitID(uint16_t id)
{
	EEPROM.update(EEPROM_UNIT_ID + 0, (static_cast<uint8_t>(id >> 0) & 0xFF));
	EEPROM.update(EEPROM_UNIT_ID + 1, (static_cast<uint8_t>(id >> 8) & 0xFF));
}

uint8_t Arduboy2Base::readUnitName(char * name)
{
	for(uint8_t dest = 0, src = EEPROM_UNIT_NAME; dest < ARDUBOY_UNIT_NAME_LEN; ++dest, ++src)
	{
		const char val = EEPROM.read(src);
		name[dest] = val;

		if((val == 0x00) || (static_cast<byte>(val) == 0xFF))
		{
			name[dest] = 0x00;
			return dest;
		}
	}

	name[ARDUBOY_UNIT_NAME_LEN] = 0x00;
	return ARDUBOY_UNIT_NAME_LEN;
}

void Arduboy2Base::writeUnitName(char * name)
{
	bool done = false;
	for(uint8_t src = 0, dest = EEPROM_UNIT_NAME; src < ARDUBOY_UNIT_NAME_LEN; ++src, ++dest)
	{
		if(name[src] == '\0')
			done = true;

		// write character or 0 pad if finished
		EEPROM.update(dest, done ? '\0' : name[src]);
	}
}

bool Arduboy2Base::readShowBootLogoFlag()
{
	return ((EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_SHOW_LOGO_MASK) != 0);
}

void Arduboy2Base::writeShowBootLogoFlag(bool val)
{
	uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);

	bitWrite(flags, SYS_FLAG_SHOW_LOGO, val);
	EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

bool Arduboy2Base::readShowUnitNameFlag()
{
	return ((EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_UNAME_MASK) != 0);
}

void Arduboy2Base::writeShowUnitNameFlag(bool val)
{
	uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);

	bitWrite(flags, SYS_FLAG_UNAME, val);
	EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

bool Arduboy2Base::readShowBootLogoLEDsFlag()
{
	return ((EEPROM.read(EEPROM_SYS_FLAGS) & SYS_FLAG_SHOW_LOGO_LEDS_MASK) != 0);
}

void Arduboy2Base::writeShowBootLogoLEDsFlag(bool val)
{
	uint8_t flags = EEPROM.read(EEPROM_SYS_FLAGS);

	bitWrite(flags, SYS_FLAG_SHOW_LOGO_LEDS, val);
	EEPROM.update(EEPROM_SYS_FLAGS, flags);
}

void Arduboy2Base::swap(int16_t & a, int16_t & b)
{
	const int16_t temp = a;
	a = b;
	b = temp;
}


//====================================
//========== class Arduboy2 ==========
//====================================

Arduboy2::Arduboy2()
{
	cursor_x = 0;
	cursor_y = 0;
	textColor = 1;
	textBackground = 0;
	textSize = 1;
	textWrap = 0;
}

// bootLogoText() should be kept in sync with bootLogoShell()
// if changes are made to one, equivalent changes should be made to the other
void Arduboy2::bootLogoText()
{
	if(!readShowBootLogoFlag())
		return;

	const bool showLEDs = readShowBootLogoLEDsFlag();

	if(showLEDs)
		digitalWriteRGB(RED_LED, RGB_ON);

	for(int16_t y = -16; y <= 24; ++y)
	{
		if(pressed(RIGHT_BUTTON))
		{
			// all LEDs off
			digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
			return;
		}

		if(showLEDs && y == 4)
		{
			// red LED off
			digitalWriteRGB(RED_LED, RGB_OFF);

			// green LED on
			digitalWriteRGB(GREEN_LED, RGB_ON);
		}

		// Using display(CLEAR_BUFFER) instead of clear() may save code space.
		// The extra time it takes to repaint the previous logo isn't an issue.
		display(CLEAR_BUFFER);

		cursor_x = 23;
		cursor_y = y;

		textSize = 2;
		print(F("ARDUBOY"));
		textSize = 1;

		display();
		delayShort(11);
	}

	if(showLEDs)
	{
		// green LED off
		digitalWriteRGB(GREEN_LED, RGB_OFF);

		// blue LED on
		digitalWriteRGB(BLUE_LED, RGB_ON);
	}

	delayShort(400);
	digitalWriteRGB(BLUE_LED, RGB_OFF);

	bootLogoExtra();
}

void Arduboy2::bootLogoExtra()
{
	if(!readShowUnitNameFlag())
		return;

	uint8_t c = EEPROM.read(EEPROM_UNIT_NAME);

	if((c != 0xFF) && (c != 0x00))
	{
		cursor_x = 50;
		cursor_y = 56;

		uint8_t i = EEPROM_UNIT_NAME;
		do
		{
			write(c);
			++i;
			c = EEPROM.read(i);
		}
		while(i < (EEPROM_UNIT_NAME + ARDUBOY_UNIT_NAME_LEN));

		display();
		delayShort(1000);
	}
}

size_t Arduboy2::write(uint8_t c)
{
	if(c == '\n')
	{
		cursor_y += (textSize * 8);
		cursor_x = 0;
	}
	else if(c == '\r')
	{
		// skip em
	}
	else
	{
		drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);

		cursor_x += (textSize * 6);

		if(textWrap && (cursor_x > (WIDTH - (textSize * 6))))
		{
			// calling ourselves recursively for 'newline' is
			// 12 bytes smaller than doing the same math here
			write('\n');
		}
	}
	return 1;
}

void Arduboy2::drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size)
{
	if((x >= WIDTH) || (y >= HEIGHT) || ((x + (5 * size) - 1) < 0) || ((y + (8 * size) - 1) < 0))
		return;

	const unsigned char * bitmap = &font[c * 5];
	const bool draw_background = (bg != color);

	for(uint8_t i = 0; i < 6; ++i)
	{
		uint8_t line = (i < 5) ? pgm_read_byte(bitmap) : 0;
		++bitmap;

		for(uint8_t j = 0; j < 8; ++j)
		{
			const uint8_t draw_color = (line & 0x1) ? color : bg;

			if(draw_color != 0 || draw_background)
				for(uint8_t a = 0; a < size; ++a)
					for(uint8_t b = 0; b < size; ++b)
						drawPixel(x + (i * size) + a, y + (j * size) + b, draw_color);

			line >>= 1;
		}
	}
}

void Arduboy2::setCursor(int16_t x, int16_t y)
{
	cursor_x = x;
	cursor_y = y;
}

int16_t Arduboy2::getCursorX()
{
	return cursor_x;
}

int16_t Arduboy2::getCursorY()
{
	return cursor_y;
}

void Arduboy2::setTextColor(uint8_t color)
{
	textColor = color;
}

uint8_t Arduboy2::getTextColor()
{
	return textColor;
}

void Arduboy2::setTextBackground(uint8_t bg)
{
	textBackground = bg;
}

uint8_t Arduboy2::getTextBackground()
{
	return textBackground;
}

void Arduboy2::setTextSize(uint8_t s)
{
	// size must always be 1 or higher
	textSize = std::max<uint8_t>(1, s);
}

uint8_t Arduboy2::getTextSize()
{
	return textSize;
}

void Arduboy2::setTextWrap(bool w)
{
	textWrap = w;
}

bool Arduboy2::getTextWrap()
{
	return textWrap;
}

void Arduboy2::clear()
{
	Arduboy2Base::clear();
	cursor_x = 0;
	cursor_y = 0;
}

