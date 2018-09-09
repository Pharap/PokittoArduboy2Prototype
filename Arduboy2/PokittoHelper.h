#pragma once

#include <cstddef>
#include <cstdint>

namespace Pokitto
{
	namespace Arduboy2Helper
	{
		static constexpr std::size_t Width = 128;
		static constexpr std::size_t Height = 64;
		static constexpr std::size_t BufferSize = (Width * Height) / 8;
	};
}