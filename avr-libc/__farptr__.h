#pragma once

//
// Includes
//

#include <cstdint>
#include <type_traits>

//
// Non-standard type aliases
//

// Ensure that the types are at least 32 bits in length, and large enough to store a pointer
using int_farptr_t = typename std::conditional<(sizeof(std::intptr_t) >= sizeof(std::int32_t)), std::intptr_t, std::int32_t>::type;
using uint_farptr_t = typename std::conditional<(sizeof(std::uintptr_t) >= sizeof(std::uint32_t)), std::uintptr_t, std::uint32_t>::type;