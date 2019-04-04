#include "ArduboyUtilities.h"

#include <Pokitto.h>

namespace Pokitto
{
	namespace ArduboyUtilities
	{
		constexpr std::size_t PokittoDisplayWidth = LCDWIDTH;
		constexpr std::size_t PokittoDisplayHeight = LCDHEIGHT;

		constexpr std::size_t BaseX = (PokittoDisplayWidth / 2) - (ArduboyDisplayWidth / 2);
		constexpr std::size_t BaseY = (PokittoDisplayHeight / 2) - (ArduboyDisplayHeight / 2);

		namespace Display
		{
			void update(void)
			{
				Pokitto::Display::update();
			}

			void drawBuffer_2bpp(const std::uint8_t * buffer);
			void drawBufferAndClear_2bpp(std::uint8_t * buffer);

			void drawBuffer_4bpp(const std::uint8_t * buffer);
			void drawBufferAndClear_4bpp(std::uint8_t * buffer);

			void drawBuffer(const std::uint8_t * buffer)
			{
				#if (POK_SCREENMODE == MODE_HI_4COLOR)
				drawBuffer_2bpp(buffer);
				#elif (POK_SCREENMODE == MODE15)
				drawBuffer_4bpp(buffer);
				#else
				static_assert(false, "Invalid screen mode for Arduboy2");
				#endif
			}

			void drawBufferAndClear(std::uint8_t * buffer)
			{
				#if (POK_SCREENMODE == MODE_HI_4COLOR)
				drawBufferAndClear_2bpp(buffer);
				#elif (POK_SCREENMODE == MODE15)
				drawBufferAndClear_4bpp(buffer);
				#else
				static_assert(false, "Invalid screen mode for Arduboy2");
				#endif
			}

			void drawBuffer(std::uint8_t * buffer, bool clear)
			{
				if(clear)
					drawBufferAndClear(buffer);
				else
					drawBuffer(buffer);
			}

			void fillDisplay(std::uint8_t colour)
			{
				Pokitto::Display::setColor((colour == 0) ? 0 : 1);
				Pokitto::Display::fillRectangle(BaseX, BaseY, ArduboyDisplayWidth, ArduboyDisplayHeight);
			}


			void drawBuffer_2bpp(const std::uint8_t * buffer)
			{
				constexpr std::size_t ArduboyBufferHeight = (ArduboyDisplayHeight / BitsPerByte);
				constexpr std::size_t Divisor2bpp = (BitsPerByte / 2);

				const std::size_t screenBufferWidth = (PokittoDisplayWidth / Divisor2bpp);
				const std::size_t baseX = (BaseX / Divisor2bpp);

				for(std::size_t y = 0; y < ArduboyBufferHeight; ++y)
				{
					const std::size_t rowBase = (BaseY + (y * BitsPerByte));
					const std::size_t columnOffsetBase = (y * ArduboyDisplayWidth);

					for(std::size_t x = 0; x < (ArduboyDisplayWidth / Divisor2bpp); ++x)
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
							Pokitto::Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

							column0 >>= 1;
							column1 >>= 1;
							column2 >>= 1;
							column3 >>= 1;
						}
					}
				}
			}

			void drawBufferAndClear_2bpp(std::uint8_t * buffer)
			{
				constexpr std::size_t ArduboyBufferHeight = (ArduboyDisplayHeight / BitsPerByte);
				constexpr std::size_t Divisor2bpp = (BitsPerByte / 2);

				const std::size_t screenBufferWidth = (PokittoDisplayWidth / Divisor2bpp);
				const std::size_t baseX = (BaseX / Divisor2bpp);

				for(std::size_t y = 0; y < ArduboyBufferHeight; ++y)
				{
					const std::size_t rowBase = (BaseY + (y * BitsPerByte));
					const std::size_t columnOffsetBase = (y * ArduboyDisplayWidth);

					for(std::size_t x = 0; x < (ArduboyDisplayWidth / Divisor2bpp); ++x)
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
							Pokitto::Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

							column0 >>= 1;
							column1 >>= 1;
							column2 >>= 1;
							column3 >>= 1;
						}
					}
				}
			}

			void drawBuffer_4bpp(const std::uint8_t * buffer)
			{
				constexpr std::size_t ArduboyBufferHeight = (ArduboyDisplayHeight / BitsPerByte);
				constexpr std::size_t Divisor4bpp = (BitsPerByte / 4);

				const std::size_t screenBufferWidth = (PokittoDisplayWidth / Divisor4bpp);
				const std::size_t baseX = (BaseX / Divisor4bpp);

				for(std::size_t y = 0; y < ArduboyBufferHeight; ++y)
				{
					const std::size_t rowBase = (BaseY + (y * BitsPerByte));
					const std::size_t columnOffsetBase = (y * ArduboyDisplayWidth);

					for(std::size_t x = 0; x < (ArduboyDisplayWidth / Divisor4bpp); ++x)
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
							Pokitto::Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

							column0 >>= 1;
							column1 >>= 1;
						}
					}
				}
			}

			void drawBufferAndClear_4bpp(std::uint8_t * buffer)
			{
				constexpr std::size_t ArduboyBufferHeight = (ArduboyDisplayHeight / BitsPerByte);
				constexpr std::size_t Divisor4bpp = (BitsPerByte / 4);

				const std::size_t screenBufferWidth = (PokittoDisplayWidth / Divisor4bpp);
				const std::size_t baseX = (BaseX / Divisor4bpp);

				for(std::size_t y = 0; y < ArduboyBufferHeight; ++y)
				{
					const std::size_t rowBase = (BaseY + (y * BitsPerByte));
					const std::size_t columnOffsetBase = (y * ArduboyDisplayWidth);

					for(std::size_t x = 0; x < (ArduboyDisplayWidth / Divisor4bpp); ++x)
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
							Pokitto::Display::screenbuffer[column + baseX + x] = static_cast<std::uint8_t>(byte);

							column0 >>= 1;
							column1 >>= 1;
						}
					}
				}
			}
		}
	};
}
