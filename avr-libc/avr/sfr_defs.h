#pragma once

//
// Macros
//

#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) static_assert(false, "bit_is_set is not implemented")
#define bit_is_clear(sfr, bit) static_assert(false, "bit_is_clear is not implemented")
#define loop_until_bit_is_set(sfr, bit) static_assert(false, "loop_until_bit_is_set is not implemented")
#define loop_until_bit_is_clear(sfr, bit) static_assert(false, "loop_until_bit_is_clear is not implemented")