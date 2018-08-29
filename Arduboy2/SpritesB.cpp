/**
 * @file SpritesB.cpp
 * \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 * Optimized for small code size.
 */

#include "SpritesB.h"

void SpritesB::drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask, uint8_t frame, uint8_t mask_frame)
{
	draw(x, y, bitmap, frame, mask, mask_frame, SPRITE_MASKED);
}

void SpritesB::drawOverwrite(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_OVERWRITE);
}

void SpritesB::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK_ERASE);
}

void SpritesB::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK);
}

void SpritesB::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
	draw(x, y, bitmap, frame, NULL, 0, SPRITE_PLUS_MASK);
}


//common functions
void SpritesB::draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame, const uint8_t *mask, uint8_t sprite_frame, uint8_t drawMode)
{
	if(bitmap == NULL)
		return;

	const uint8_t width = pgm_read_byte(bitmap);
	++bitmap;
	const uint8_t height = pgm_read_byte(bitmap);
	++bitmap;
	if(frame > 0 || sprite_frame > 0)
	{
		unsigned int frame_offset = (width * ( height / 8 + ( height % 8 == 0 ? 0 : 1)));
		// sprite plus mask uses twice as much space for each frame
		if(drawMode == SPRITE_PLUS_MASK)
			frame_offset *= 2;
		else if(mask != NULL)
			mask += sprite_frame * frame_offset;
		bitmap += frame * frame_offset;
	}

	// if we're detecting the draw mode then base it on whether a mask
	// was passed as a separate object
	if(drawMode == SPRITE_AUTO_MODE)
		drawMode = mask == NULL ? SPRITE_UNMASKED : SPRITE_MASKED;

	drawBitmap(x, y, bitmap, mask, width, height, drawMode);
}

void SpritesB::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask, uint8_t w, uint8_t h, uint8_t draw_mode)
{
	// no need to draw at all of we're offscreen
	if(x + w <= 0 || x > WIDTH - 1 || y + h <= 0 || y > HEIGHT - 1)
		return;

	if(bitmap == NULL)
		return;

	// xOffset technically doesn't need to be 16 bit but the math operations
	// are measurably faster if it is
	// if the left side of the render is offscreen skip those loops
	const uint16_t xOffset = (x < 0) ? abs(x) : 0;

	// if the right side of the render is offscreen skip those loops
	const uint8_t rendered_width = (x + w > WIDTH - 1) ? ((WIDTH - x) - xOffset) : (w - xOffset);

	const int8_t yOffset = y & 7;
	const int8_t tempSRow = y / 8;
	int8_t sRow = (y < 0 && yOffset > 0) ? tempSRow - 1 : tempSRow;

	// if the top side of the render is offscreen skip those loops
	const uint8_t start_h = (sRow < -1) ? abs(sRow) - 1 : 0;

	// divide, then round up
	const uint8_t rows = h / 8;
	uint8_t loop_h = ((h % 8) > 0) ? (rows + 1) : rows;

	// if(sRow + loop_h - 1 > (HEIGHT/8)-1)
	if(sRow + loop_h > (HEIGHT / 8))
		loop_h = (HEIGHT / 8) - sRow;

	// prepare variables for loops later so we can compare with 0
	// instead of comparing two variables
	loop_h -= start_h;
	sRow += start_h;

	const uint8_t mul_amt = 1 << yOffset;

	const uint8_t ofs_step = (draw_mode == SPRITE_PLUS_MASK) ? 2 : 1;
	const uint8_t ofs_stride = (w - rendered_width) * ofs_step;
	const uint16_t initial_bofs = ((start_h * w) + xOffset) * ofs_step;

	uint16_t ofs = (sRow * WIDTH) + x + xOffset;
	const uint8_t * bofs = bitmap + initial_bofs;
	const uint8_t * mask_ofs = ((mask == NULL) ? bitmap : mask) + initial_bofs + ofs_step - 1;

	for(uint8_t a = 0; a < loop_h; ++a)
	{
		for(uint8_t iCol = 0; iCol < rendered_width; ++iCol)
		{
			uint16_t bitmap_data = pgm_read_byte(bofs) * mul_amt;
			uint16_t mask_data = ~(pgm_read_byte(mask_ofs) * mul_amt);

			if(draw_mode == SPRITE_IS_MASK_ERASE)
			{
				mask_data = ~bitmap_data;
				bitmap_data = 0;
			}
			else if(draw_mode == SPRITE_UNMASKED)
				mask_data = ~(0xFF * mul_amt);

			if(sRow >= 0)
			{
				uint8_t data = Arduboy2Base::sBuffer[ofs];
				data &= static_cast<uint8_t>(mask_data);
				data |= static_cast<uint8_t>(bitmap_data);
				Arduboy2Base::sBuffer[ofs] = data;
			}
			if(yOffset != 0 && sRow < 7)
			{
				uint8_t data = Arduboy2Base::sBuffer[ofs + WIDTH];
				data &= (*(reinterpret_cast<unsigned char *>(&mask_data) + 1));
				data |= (*(reinterpret_cast<unsigned char *>(&bitmap_data) + 1));
				Arduboy2Base::sBuffer[ofs + WIDTH] = data;
			}
			++ofs;
			mask_ofs += ofs_step;
			bofs += ofs_step;
		}
		++sRow;
		bofs += ofs_stride;
		mask_ofs += ofs_stride;
		ofs += WIDTH - rendered_width;
	}
}
