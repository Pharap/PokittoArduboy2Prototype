#pragma once

#include "PokittoHelper.h"

#include <Pokitto.h>

#include <climits>

namespace Pokitto
{
	namespace Arduboy2Helper
	{		
		static constexpr std::size_t BaseX = (LCDWIDTH / 2) - (Width / 2);
		static constexpr std::size_t BaseY = (LCDHEIGHT / 2) - (Height / 2);
		
		constexpr std::size_t BitsPerByte = CHAR_BIT;
		
		void drawArduboy2Buffer_2bpp(const std::uint8_t * buffer);
		void drawArduboy2BufferAndClear_2bpp(std::uint8_t * buffer);
		
		void drawArduboy2Buffer_4bpp(const std::uint8_t * buffer);
		void drawArduboy2BufferAndClear_4bpp(std::uint8_t * buffer);
		
		void drawArduboy2Buffer(const std::uint8_t * buffer)
		{
			#if (POK_SCREENMODE == MODE_HI_4COLOR)
			drawArduboy2Buffer_2bpp(buffer);
			#elif (POK_SCREENMODE == MODE15)
			drawArduboy2Buffer_4bpp(buffer);
			#else
			static_assert(false, "Invalid screen mode for Arduboy2");
			#endif

			Pokitto::Display::update();
		}
		
		void drawArduboy2BufferAndClear(std::uint8_t * buffer)
		{
			#if (POK_SCREENMODE == MODE_HI_4COLOR)
			drawArduboy2BufferAndClear_2bpp(buffer);
			#elif (POK_SCREENMODE == MODE15)
			drawArduboy2BufferAndClear_4bpp(buffer);
			#else
			static_assert(false, "Invalid screen mode for Arduboy2");
			#endif

			Pokitto::Display::update();
		}
		
		void drawArduboy2Buffer(std::uint8_t * buffer, bool clear)
		{
			if(clear)
				drawArduboy2BufferAndClear(buffer);
			else
				drawArduboy2Buffer(buffer);
		}
		
		void fillArduboyScreen(std::uint8_t colour)
		{		
			Pokitto::Display::setColor((colour == 0) ? 0 : 1);
			Pokitto::Display::fillRectangle(BaseX, BaseY, Width, Height);
			Pokitto::Display::update();
		}

		constexpr std::size_t Divisor2bpp = (BitsPerByte / 2);
		
		void drawArduboy2Buffer_2bpp(const std::uint8_t * buffer)
		{
			const std::size_t screenBufferWidth = (LCDWIDTH / Divisor2bpp);
			const std::size_t baseX = (BaseX / Divisor2bpp);

			for(std::size_t y = 0; y < (Height / BitsPerByte); ++y)
			{
				const std::size_t rowBase = (BaseY + (y * BitsPerByte));
				const std::size_t columnOffsetBase = (y * Width);

				for(std::size_t x = 0; x < (Width / Divisor2bpp); ++x)
				{
					const std::size_t columnOffset = columnOffsetBase + (x * Divisor2bpp);

					std::uint_fast8_t column0 = buffer[columnOffset + 0];
					std::uint_fast8_t column1 = buffer[columnOffset + 1];
					std::uint_fast8_t column2 = buffer[columnOffset + 2];
					std::uint_fast8_t column3 = buffer[columnOffset + 3];

					for(std::size_t i = 0; i < BitsPerByte; ++i)
					{
						const std::uint_fast8_t bit0 = ((column0 & 0x01) << 6);
						const std::uint_fast8_t bit1 = ((column1 & 0x01) << 4);
						const std::uint_fast8_t bit2 = ((column2 & 0x01) << 2);
						const std::uint_fast8_t bit3 = ((column3 & 0x01) << 0);

						const std::uint_fast8_t byte = (bit0 | bit1 | bit2 | bit3);

						const std::size_t column = ((rowBase + i) * screenBufferWidth);
						Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

						column0 >>= 1;
						column1 >>= 1;
						column2 >>= 1;
						column3 >>= 1;
					}
				}
			}
		}
		
		void drawArduboy2BufferAndClear_2bpp(std::uint8_t * buffer)
		{
			const std::size_t screenBufferWidth = (LCDWIDTH / Divisor2bpp);
			const std::size_t baseX = (BaseX / Divisor2bpp);

			for(std::size_t y = 0; y < (Height / BitsPerByte); ++y)
			{
				const std::size_t rowBase = (BaseY + (y * BitsPerByte));
				const std::size_t columnOffsetBase = (y * Width);

				for(std::size_t x = 0; x < (Width / Divisor2bpp); ++x)
				{
					const std::size_t columnOffset = columnOffsetBase + (x * Divisor2bpp);

					std::uint_fast8_t column0 = buffer[columnOffset + 0];
					std::uint_fast8_t column1 = buffer[columnOffset + 1];
					std::uint_fast8_t column2 = buffer[columnOffset + 2];
					std::uint_fast8_t column3 = buffer[columnOffset + 3];

					buffer[columnOffset + 0] = 0;
					buffer[columnOffset + 1] = 0;
					buffer[columnOffset + 2] = 0;
					buffer[columnOffset + 3] = 0;

					for(std::size_t i = 0; i < BitsPerByte; ++i)
					{
						const std::uint_fast8_t bit0 = ((column0 & 0x01) << 6);
						const std::uint_fast8_t bit1 = ((column1 & 0x01) << 4);
						const std::uint_fast8_t bit2 = ((column2 & 0x01) << 2);
						const std::uint_fast8_t bit3 = ((column3 & 0x01) << 0);

						const std::uint_fast8_t byte = (bit0 | bit1 | bit2 | bit3);

						const std::size_t column = ((rowBase + i) * screenBufferWidth);
						Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

						column0 >>= 1;
						column1 >>= 1;
						column2 >>= 1;
						column3 >>= 1;
					}
				}
			}
		}

		constexpr std::size_t Divisor4bpp = (BitsPerByte / 4);
		
		void drawArduboy2Buffer_4bpp(const std::uint8_t * buffer)
		{
			const std::size_t screenBufferWidth = (LCDWIDTH / Divisor4bpp);
			const std::size_t baseX = (BaseX / Divisor4bpp);

			for(std::size_t y = 0; y < (Height / BitsPerByte); ++y)
			{
				const std::size_t rowBase = (BaseY + (y * BitsPerByte));
				const std::size_t columnOffsetBase = (y * Width);

				for(std::size_t x = 0; x < (Width / Divisor4bpp); ++x)
				{
					const std::size_t columnOffset = columnOffsetBase + (x * Divisor4bpp);

					std::uint_fast8_t column0 = buffer[columnOffset + 0];
					std::uint_fast8_t column1 = buffer[columnOffset + 1];

					for(std::size_t i = 0; i < BitsPerByte; ++i)
					{
						const std::uint_fast8_t bit0 = ((column0 & 0x01) << 4);
						const std::uint_fast8_t bit1 = ((column1 & 0x01) << 0);

						const std::uint_fast8_t byte = (bit0 | bit1);

						const std::size_t column = ((rowBase + i) * screenBufferWidth);
						Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

						column0 >>= 1;
						column1 >>= 1;
					}
				}
			}
		}
		
		void drawArduboy2BufferAndClear_4bpp(std::uint8_t * buffer)
		{
			const std::size_t screenBufferWidth = (LCDWIDTH / Divisor4bpp);
			const std::size_t baseX = (BaseX / Divisor4bpp);

			for(std::size_t y = 0; y < (Height / BitsPerByte); ++y)
			{
				const std::size_t rowBase = (BaseY + (y * BitsPerByte));
				const std::size_t columnOffsetBase = (y * Width);

				for(std::size_t x = 0; x < (Width / Divisor4bpp); ++x)
				{
					const std::size_t columnOffset = columnOffsetBase + (x * Divisor4bpp);

					std::uint_fast8_t column0 = buffer[columnOffset + 0];
					std::uint_fast8_t column1 = buffer[columnOffset + 1];
					
					buffer[columnOffset + 0] = 0;
					buffer[columnOffset + 1] = 0;

					for(std::size_t i = 0; i < BitsPerByte; ++i)
					{
						const std::uint_fast8_t bit0 = ((column0 & 0x01) << 4);
						const std::uint_fast8_t bit1 = ((column1 & 0x01) << 0);

						const std::uint_fast8_t byte = (bit0 | bit1);

						const std::size_t column = ((rowBase + i) * screenBufferWidth);
						Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

						column0 >>= 1;
						column1 >>= 1;
					}
				}
			}
		}
	};
}