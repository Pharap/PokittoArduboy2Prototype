#pragma once

#include "Arduboy2.h"

#include <cstddef>
#include <cstdint>

namespace Pokitto
{
	namespace Arduboy2Helper
	{
		constexpr std::size_t BitsPerByte = 8;

		constexpr std::size_t ArduboyDisplayWidth = WIDTH;
		constexpr std::size_t ArduboyDisplayHeight = HEIGHT;
		constexpr std::size_t ArduboyDisplaySize = (ArduboyDisplayWidth * ArduboyDisplayHeight) / BitsPerByte;

		namespace Display
		{
			void drawBuffer(const std::uint8_t * buffer);
			void drawBuffer(std::uint8_t * buffer, bool clear);
			void fillDisplay(std::uint8_t colour);
		}
	};
}