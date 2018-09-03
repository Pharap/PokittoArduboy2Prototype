#pragma once

//
// Includes
//

#include <cstddef>
#include <cstdint>

#include "../__attrs__.h"
#include "../__farptr__.h"

//
// Macros
//

#define PROGMEM
#define PGM_P const char *
#define PGM_VOID_P const void *
#define PSTR(s) ((const PROGMEM char *)(s))

//
// Progmem read near macros
//

#define pgm_read_byte_near(address_short) (*((const std::uint8_t *)(address_short)))
#define pgm_read_word_near(address_short) (*((const std::uint16_t *)(address_short)))
#define pgm_read_dword_near(address_short) (*((const std::uint32_t *)(address_short)))
#define pgm_read_float_near(address_short) (*((const float *)(address_short)))
#define pgm_read_ptr_near(address_short) (void*)(*((const void * const *)(address_short)))

//
// Progmem read far macros
//

#define pgm_read_byte_far(address_long) (*((const std::uint8_t *)(address_long)))
#define pgm_read_word_far(address_long) (*((const std::uint16_t *)(address_long)))
#define pgm_read_dword_far(address_long) (*((const std::uint32_t *)(address_long)))
#define pgm_read_float_far(address_long) (*((const float *)(address_long)))
#define pgm_read_ptr_far(address_long) (void*)(*((const void * const *)(address_long)))

//
// Progmem read macros
//

#define pgm_read_byte(address_short) pgm_read_byte_near(address_short)
#define pgm_read_word(address_short) pgm_read_word_near(address_short)
#define pgm_read_dword(address_short) pgm_read_dword_near(address_short)
#define pgm_read_float(address_short) pgm_read_float_near(address_short)
#define pgm_read_ptr(address_short) pgm_read_ptr_near(address_short)

//
// Progmem get far
//

#define pgm_get_far_address(var) (reinterpret_cast<uint_farptr_t>(var) & reinterpret_cast<uint_farptr_t>(0x00FFFFFF))

//
// Type Aliases
//

// Interesting that the original assumes char is signed,
// neither the C++ or the C standard make such a guarantee.

using prog_void = void PROGMEM;
using prog_char = char PROGMEM;
using prog_uchar = unsigned char PROGMEM;
using prog_int8_t = std::int8_t PROGMEM;
using prog_uint8_t = std::uint8_t PROGMEM;
using prog_int16_t = std::int16_t PROGMEM;
using prog_uint16_t = std::uint16_t PROGMEM;
using prog_int32_t = std::int32_t PROGMEM;
using prog_uint32_t = std::uint32_t PROGMEM;
using prog_int64_t = std::int64_t PROGMEM;
using prog_uint64_t = std::uint64_t PROGMEM;

//
// String manipulation functions
//

char * strcpy_P(char * dest, const char * src);
char * strcpy_PF(char * dest, uint_farptr_t src);

char * strncpy_P(char * dest, const char * src, std::size_t count);
char * strncpy_PF(char * dest, uint_farptr_t src, std::size_t count);

char * strcat_P(char * dest, const char * src);
char * strcat_PF(char * dest, uint_farptr_t src);

char * strncat_P(char * dest, const char * src, std::size_t count);
char * strncat_PF(char * dest, uint_farptr_t src, std::size_t count);

//
// Extra string manipulation functions
//

std::size_t strlcpy_P(char * dest, const char * src, std::size_t count);
std::size_t strlcpy_PF(char * dest, uint_farptr_t src, std::size_t count);

std::size_t strlcat_P(char * dest, const char * src, std::size_t count);
std::size_t strlcat_PF(char * dest, uint_farptr_t src, std::size_t count);

//
// String examination functions
//

std::size_t strlen_P(const char * str);
std::size_t strlen_PF(uint_farptr_t str);

__ATTR_PURE__ int strcmp_P(const char * lhs, const char * rhs);
__ATTR_PURE__ int strcmp_PF(const char * lhs, uint_farptr_t rhs);

__ATTR_PURE__ int strncmp_P(const char * lhs, const char * rhs, std::size_t count);
__ATTR_PURE__ int strncmp_PF(const char * lhs, uint_farptr_t rhs, std::size_t count);

const char * strchr_P(const char * str, int ch);

const char * strrchr_P(const char * str, int ch);

__ATTR_PURE__ std::size_t strspn_P(const char * dest, const char * src);

__ATTR_PURE__ std::size_t strcspn_P(const char * dest, const char * src);

__ATTR_PURE__ char * strpbrk_P(const char * dest, const char * breakset);

__ATTR_PURE__ char * strstr_P(const char * str, const char * target);
char * strstr_PF(const char * str, uint_farptr_t target);

char * strtok_P(char * str, const char * delim);

//
// Extra string examination functions
//

std::size_t strnlen_P(const char * str, std::size_t count) = delete;
std::size_t strnlen_PF(uint_farptr_t str, std::size_t count) = delete;

__ATTR_PURE__ int strcasecmp_P(const char * lhs, const char * rhs) = delete;
__ATTR_PURE__ int strcasecmp_PF(const char * lhs, uint_farptr_t rhs) = delete;

__ATTR_PURE__ int strncasecmp_P(const char * lhs, const char * rhs, std::size_t count) = delete;
__ATTR_PURE__ int strncasecmp_PF(const char * lhs, uint_farptr_t rhs, std::size_t count) = delete;

const char * strchrnul_P(const char * str, int ch) = delete;

__ATTR_PURE__ char * strcasestr_P(const char * str, const char * target) = delete;

char * strtok_rP(char * str, const char * delim, char *  * context) = delete;

//
// Character array manipulation functions
//

void * memchr_P(const void * ptr, int ch, std::size_t count);

__ATTR_PURE__ int memcmp_P(const void * lhs, const void * rhs, std::size_t count);
__ATTR_PURE__ int memcmp_PF(const void * lhs, uint_farptr_t rhs, std::size_t count);

void * memcpy_P(void * dest, const void * src, std::size_t count);
void * memcpy_PF(void * dest, uint_farptr_t src, std::size_t count);

//
// Extra character array manipulation functions
//

const void * memrchr_P(const void * src, int ch, std::size_t count) = delete;

void * memccpy_P(void * dest, const void * src, int ch, std::size_t count) = delete;

__ATTR_PURE__ void * memmem_P(const void * src, std::size_t src_count, const void * target, std::size_t target_count) = delete;

//
// Extra miscellaneous function
//

char * strsep_P(char *  * strptr, const char * delim) = delete;

