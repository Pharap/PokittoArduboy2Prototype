#pragma once

#include "PokittoHelper.h"

#include <Pokitto.h>

namespace Pokitto
{
	namespace Arduboy2Helper
	{		
		static constexpr std::size_t BaseX = (220 / 2) - (Width / 2);
		static constexpr std::size_t BaseY = (176 / 2) - (Height / 2);
	};
}