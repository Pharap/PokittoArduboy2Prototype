/**
 * @file Sprites.cpp
 * \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 */

#include "Sprites.h"

#include <cstdlib>

void Sprites::drawExternalMask(int16_t x, int16_t y, const uint8_t * bitmap, const uint8_t * mask, uint8_t frame, uint8_t mask_frame)
{
	draw(x, y, bitmap, frame, mask, mask_frame, SPRITE_MASKED);
}

void Sprites::drawOverwrite(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_OVERWRITE);
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK_ERASE);
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK);
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_PLUS_MASK);
}


//common functions
void Sprites::draw(int16_t x, int16_t y, const uint8_t * bitmap, uint8_t frame, const uint8_t * mask, uint8_t sprite_frame, uint8_t drawMode)
{
	if(bitmap == NULL)
		return;

	const uint8_t width = pgm_read_byte(bitmap);
	++bitmap;

	const uint8_t height = pgm_read_byte(bitmap);
	++bitmap;

	if((frame > 0) || (sprite_frame > 0))
	{
		unsigned int frame_offset = (width * ((height / 8) + (((height % 8) == 0) ? 0 : 1)));

		// sprite plus mask uses twice as much space for each frame
		if(drawMode == SPRITE_PLUS_MASK)
			frame_offset *= 2;
		else if(mask != NULL)
			mask += (sprite_frame * frame_offset);

		bitmap += (frame * frame_offset);
	}

	// if we're detecting the draw mode then base it on whether a mask
	// was passed as a separate object
	if(drawMode == SPRITE_AUTO_MODE)
		drawMode = (mask == NULL) ? SPRITE_UNMASKED : SPRITE_MASKED;

	drawBitmap(x, y, bitmap, mask, width, height, drawMode);
}

void Sprites::drawBitmap(int16_t x, int16_t y, const uint8_t * bitmap, const uint8_t * mask, uint8_t w, uint8_t h, uint8_t draw_mode)
{
	// no need to draw at all of we're offscreen
	if(((x + w) <= 0) || (x > (WIDTH - 1)) || ((y + h) <= 0) || (y > (HEIGHT - 1)))
		return;

	if(bitmap == NULL)
		return;

	// xOffset technically doesn't need to be 16 bit but the math operations
	// are measurably faster if it is
	// if the left side of the render is offscreen skip those loops
	const uint16_t xOffset = (x < 0) ? std::abs(x) : 0;

	// if the right side of the render is offscreen skip those loops
	const uint8_t rendered_width = ((x + w) > (WIDTH - 1)) ? ((WIDTH - x) - xOffset) : (w - xOffset);

	const uint8_t yOffset = (y & 7);
	const int8_t tempSRow = (y / 8);
	int8_t sRow = ((y < 0) && (yOffset > 0)) ? (tempSRow - 1) : tempSRow;

	// if the top side of the render is offscreen skip those loops
	const uint8_t start_h = (sRow < -1) ? (std::abs(sRow) - 1) : 0;

	// divide, then round up
	const uint8_t rows = (h / 8);
	uint8_t loop_h = ((h % 8) > 0) ? (rows + 1) : rows;

	// if(sRow + loop_h - 1 > (HEIGHT/8)-1)
	if((sRow + loop_h) > (HEIGHT / 8))
		loop_h = ((HEIGHT / 8) - sRow);

	// prepare variables for loops later so we can compare with 0
	// instead of comparing two variables
	loop_h -= start_h;
	sRow += start_h;

	const uint8_t mul_amt = (1 << yOffset);
	int16_t ofs = ((sRow * WIDTH) + x + xOffset);
	const uint8_t * bofs = &bitmap[(start_h * w) + xOffset];

	switch (draw_mode)
	{
		case SPRITE_UNMASKED:
		{
			// we only want to mask the 8 bits of our own sprite, so we can
			// calculate the mask before the start of the loop
			const uint16_t mask_data = ~(0xFF * mul_amt);
			// really if yOffset = 0 you have a faster case here that could be
			// optimized
			for(uint8_t a = 0; a < loop_h; ++a)
			{
				for(uint8_t iCol = 0; iCol < rendered_width; ++iCol)
				{
					const uint16_t bitmap_data = (pgm_read_byte(bofs) * mul_amt);

					if(sRow >= 0)
					{
						uint8_t data = Arduboy2Base::sBuffer[ofs];
						data &= static_cast<uint8_t>(mask_data >> 0);
						data |= static_cast<uint8_t>(bitmap_data >> 0);
						Arduboy2Base::sBuffer[ofs] = data;
					}

					if((yOffset != 0) && (sRow < 7))
					{
						const std::size_t index = static_cast<uint16_t>(ofs + WIDTH);
						uint8_t data = Arduboy2Base::sBuffer[index];
						data &= static_cast<uint8_t>(mask_data >> 8);
						data |= static_cast<uint8_t>(bitmap_data >> 8);
						Arduboy2Base::sBuffer[index] = data;
					}

					++ofs;
					++bofs;
				}

				++sRow;
				bofs += (w - rendered_width);
				ofs += (WIDTH - rendered_width);
			}
			break;
		}

		case SPRITE_IS_MASK:
		{
			for(uint8_t a = 0; a < loop_h; ++a)
			{
				for(uint8_t iCol = 0; iCol < rendered_width; ++iCol)
				{
					const uint16_t bitmap_data = (pgm_read_byte(bofs) * mul_amt);

					if(sRow >= 0)
						Arduboy2Base::sBuffer[ofs] |= static_cast<uint8_t>(bitmap_data >> 0);

					if((yOffset != 0) && (sRow < 7))
						Arduboy2Base::sBuffer[ofs + WIDTH] |= static_cast<uint8_t>(bitmap_data >> 8);

					++ofs;
					++bofs;
				}

				++sRow;
				bofs += (w - rendered_width);
				ofs += (WIDTH - rendered_width);
			}
			break;
		}

		case SPRITE_IS_MASK_ERASE:
		{
			for(uint8_t a = 0; a < loop_h; ++a)
			{
				for(uint8_t iCol = 0; iCol < rendered_width; ++iCol)
				{
					const uint16_t bitmap_data = (pgm_read_byte(bofs) * mul_amt);

					if(sRow >= 0)
						Arduboy2Base::sBuffer[ofs]  &= ~static_cast<uint8_t>(bitmap_data >> 0);

					if((yOffset != 0) && (sRow < 7))
					{
						const std::size_t index = static_cast<uint16_t>(ofs + WIDTH);
						Arduboy2Base::sBuffer[index] &= ~static_cast<uint8_t>(bitmap_data >> 8);
					}

					++ofs;
					++bofs;
				}

				++sRow;
				bofs += (w - rendered_width);
				ofs += (WIDTH - rendered_width);
			}
			break;
		}

		case SPRITE_MASKED:
		{
			const uint8_t * mask_ofs = (mask + (start_h * w) + xOffset);

			for(uint8_t a = 0; a < loop_h; ++a)
			{
				for(uint8_t iCol = 0; iCol < rendered_width; ++iCol)
				{
					// NOTE: you might think in the yOffset==0 case that this results
					// in more effort, but in all my testing the compiler was forcing
					// 16-bit math to happen here anyways, so this isn't actually
					// compiling to more code than it otherwise would. If the offset
					// is 0 the high part of the word will just never be used.

					// load data and bit shift
					// mask needs to be bit flipped
					const uint16_t mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);
					const uint16_t bitmap_data = (pgm_read_byte(bofs) * mul_amt);

					if(sRow >= 0)
					{
						uint8_t data = Arduboy2Base::sBuffer[ofs];
						data &= static_cast<uint8_t>(mask_data >> 0);
						data |= static_cast<uint8_t>(bitmap_data >> 0);
						Arduboy2Base::sBuffer[ofs] = data;
					}

					if(yOffset != 0 && sRow < 7)
					{
						const std::size_t index = static_cast<uint16_t>(ofs + WIDTH);
						uint8_t data = Arduboy2Base::sBuffer[index];
						data &= static_cast<uint8_t>(mask_data >> 8);
						data |= static_cast<uint8_t>(bitmap_data >> 8);
						Arduboy2Base::sBuffer[index] = data;
					}

					++ofs;
					++mask_ofs;
					++bofs;
				}

				++sRow;
				bofs += (w - rendered_width);
				mask_ofs += (w - rendered_width);
				ofs += (WIDTH - rendered_width);
			}
			break;
		}

		case SPRITE_PLUS_MASK:
		{
			const uint8_t sprite_ofs_jump = ((w - rendered_width) * 2);
			const uint8_t buffer_ofs_jump = (WIDTH - rendered_width);

			// * 2 because we use double the bits (mask + bitmap)
			const uint8_t * sprite_ofs = &bitmap[((start_h * w) + xOffset) * 2];

			uint8_t * buffer_ofs = &Arduboy2Base::sBuffer[ofs];
			uint8_t * buffer_ofs_2 = &Arduboy2Base::sBuffer[ofs + WIDTH];

			for(uint8_t yi = loop_h; yi > 0; --yi)
			{
				for(uint8_t xi = rendered_width; xi > 0; --xi)
				{
					// load bitmap and mask data
					uint16_t bitmap_data = pgm_read_byte(sprite_ofs);
					++sprite_ofs;

					uint16_t mask_data = pgm_read_byte(sprite_ofs);
					++sprite_ofs;

					// shift mask and buffer data
					if(yOffset != 0)
					{
						// The most significant byte is guaranteed to be 0 at this point
						// due to the earlier implicit cast from unsigned char to uint16_t
						bitmap_data *= mul_amt;
						mask_data *= mul_amt;

						// SECOND PAGE
						if(sRow < 7)
						{
							const uint8_t dataMask = static_cast<uint8_t>(~mask_data >> 8);
							const uint8_t imageData = static_cast<uint8_t>(bitmap_data >> 8);
							const uint8_t data = *buffer_ofs_2;

							*buffer_ofs_2 = ((data & dataMask) | imageData);
							++buffer_ofs_2;
						}
					}

					// FIRST_PAGE
					if(sRow >= 0)
					{
						const uint8_t dataMask = static_cast<uint8_t>(~mask_data >> 0);
						const uint8_t imageData = static_cast<uint8_t>(bitmap_data >> 0);
						const uint8_t data = *buffer_ofs;

						*buffer_ofs = ((data & dataMask) | imageData);
					}

					++buffer_ofs;
				}

				++sRow;
				sprite_ofs += sprite_ofs_jump;
				buffer_ofs += buffer_ofs_jump;
				buffer_ofs_2 += buffer_ofs_jump;
			}
			break;
		}
	}
}
