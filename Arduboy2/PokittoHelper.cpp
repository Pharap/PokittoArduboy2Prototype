#pragma once

#include "PokittoHelper.h"

#include <Pokitto.h>

namespace Pokitto
{
	namespace Arduboy2Helper
	{		
		static constexpr std::size_t BaseX = (220 / 2) - (Width / 2);
		static constexpr std::size_t BaseY = (176 / 2) - (Height / 2);
		
		void drawArduboy2Buffer(const std::uint8_t * buffer)
		{
			for(std::size_t y = 0; y < (Height / 8); ++y)
			{
				const std::size_t columnBase = y * 8;
				
				for(std::size_t x = 0; x < Width; ++x)
				{
					const std::uint8_t column = buffer[(y * Width) + x];
					
					for(std::size_t i = 0, c = column; i < 8; ++i, c >>= 1)
					{
						const uint8_t bit = static_cast<std::uint8_t>(c & 0x01);
						Pokitto::Display::drawPixel(BaseX + x, BaseY + columnBase + i, bit);
					}
				}
			}
		}
		
		void drawArduboy2Buffer(std::uint8_t * buffer, bool clear)
		{		
			for(std::size_t y = 0; y < (Height / 8); ++y)
			{
				const std::size_t columnBase = y * 8;
				
				for(std::size_t x = 0; x < Width; ++x)
				{					
					const std::uint8_t column = buffer[(y * Width) + x];
					
					if(clear)
						buffer[(y * Width) + x] = 0;
					
					for(std::size_t i = 0, c = column; i < 8; ++i, c >>= 1)
					{
						const uint8_t bit = static_cast<std::uint8_t>(c & 0x01);
						Pokitto::Display::drawPixel(BaseX + x, BaseY + columnBase + i, bit);
					}					
				}
			}
		}
	};
}