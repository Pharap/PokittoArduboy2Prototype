#include "pgmspace.h"

//
// Includes
//

#include <cstring>
#include <algorithm>

//
// String manipulation functions
//

char * strcpy_P(char * dest, const char * src)
{
	return std::strcpy(dest, src);
}

char * strncpy_P(char * dest, const char * src, std::size_t count)
{
	return std::strncpy(dest, src, count);
}

char * strcat_P(char * dest, const char * src)
{
	return std::strcat(dest, src);
}

char * strncat_P(char * dest, const char * src, std::size_t count)
{
	return std::strncat(dest, src, count);
}

char * strcpy_PF(char * dest, uint_farptr_t src)
{
	return std::strcpy(dest, reinterpret_cast<const char *>(src));
}

char * strncpy_PF(char * dest, uint_farptr_t src, std::size_t count)
{
	return std::strncpy(dest, reinterpret_cast<const char *>(src), count);
}

char * strcat_PF(char * dest, uint_farptr_t src)
{
	return std::strcat(dest, reinterpret_cast<const char *>(src));
}

char * strncat_PF(char * dest, uint_farptr_t src, std::size_t count)
{
	return std::strncat(dest, reinterpret_cast<const char *>(src), count);
}

//
// Extra string manipulation functions
//

std::size_t strlcpy_P(char * dest, const char * src, std::size_t count)
{	
	const std::size_t end = count - 1;

	for (std::size_t i = 0;; ++i)
	{
		if (i < end)
			dest[i] = src[i];
		else if (i == end)
			dest[i] = '\0';

		if (src[i] == '\0')
			return i;
	}
}

std::size_t strlcpy_PF(char * dest, uint_farptr_t src, std::size_t count)
{
	return strlcpy_P(dest, reinterpret_cast<const char *>(src), count);
}

// Author's note: this function is evil
std::size_t strlcat_P(char * dest, const char * src, std::size_t count)
{
	const std::size_t end = count - 1;
	const std::size_t start = std::strlen(dest);
	
	for (std::size_t i = start, j = 0;; ++i, ++j)
	{
		if(i < end)
			dest[i] = src[j];
		else if (i == end)
			dest[i] = '\0';

		if (src[j] == '\0')
			return (j + std::min(start, count));
	}
}

std::size_t strlcat_PF(char * dest, uint_farptr_t src, std::size_t count)
{
	return strlcat_P(dest, reinterpret_cast<const char *>(src), count);
}

//
// String examination functions
//

std::size_t strlen_P(const char * str)
{
	return std::strlen(str);
}

std::size_t strlen_PF(uint_farptr_t str)
{
	return std::strlen(reinterpret_cast<const char *>(str));
}

// __ATTR_PURE__
int strcmp_P(const char * lhs, const char * rhs)
{
	return std::strcmp(lhs, rhs);
}

// __ATTR_PURE__
int strcmp_PF(const char * lhs, uint_farptr_t rhs)
{
	return std::strcmp(lhs, reinterpret_cast<const char *>(rhs));
}

// __ATTR_PURE__
int strncmp_P(const char * lhs, const char * rhs, std::size_t count)
{
	return std::strncmp(lhs, rhs, count);
}

// __ATTR_PURE__
int strncmp_PF(const char * lhs, uint_farptr_t rhs, std::size_t count)
{
	return std::strncmp(lhs, reinterpret_cast<const char *>(rhs), count);
}

const char * strchr_P(const char * str, int ch)
{
	return std::strchr(str, ch);
}

const char * strrchr_P(const char * str, int ch)
{
	return std::strrchr(str, ch);
}

// __ATTR_PURE__
std::size_t strspn_P(const char * dest, const char * src)
{
	return std::strspn(dest, src);
}

// __ATTR_PURE__
std::size_t strcspn_P(const char * dest, const char * src)
{
	return std::strcspn(dest, src);
}

// __ATTR_PURE__
char * strpbrk_P(const char * dest, const char * breakset)
{
	return std::strpbrk(dest, breakset);
}

// __ATTR_PURE__
char * strstr_P(const char * str, const char * target)
{
	return std::strstr(str, target);
}

char * strstr_PF(const char * str, uint_farptr_t target)
{
	return std::strstr(str, reinterpret_cast<const char *>(target));
}

char * strtok_P(char * str, const char * delim)
{
	return std::strtok(str, delim);
}

//
// Extra string examination functions
//

//std::size_t strnlen_P(const char * str, std::size_t count) = delete;
//std::size_t strnlen_PF(uint_farptr_t str, std::size_t count) = delete;

//__ATTR_PURE__ int strcasecmp_P(const char * lhs, const char * rhs) = delete;
//__ATTR_PURE__ int strcasecmp_PF(const char * lhs, uint_farptr_t rhs) = delete;

//__ATTR_PURE__ int strncasecmp_P(const char * lhs, const char * rhs, std::size_t count) = delete;
//__ATTR_PURE__ int strncasecmp_PF(const char * lhs, uint_farptr_t rhs, std::size_t count) = delete;

//const char * strchrnul_P(const char * str, int ch) = delete;

//__ATTR_PURE__ char * strcasestr_P(const char * str, const char * target) = delete;

//char * strtok_rP(char * str, const char * delim, char *  * context) = delete;

//
// Character array manipulation functions
//

void * memchr_P(const void * ptr, int ch, std::size_t count)
{
	return std::memchr(ptr, ch, count);
}

// __ATTR_PURE__
int memcmp_P(const void * lhs, const void * rhs, std::size_t count)
{
	return std::memcmp(lhs, rhs, count);
}

// __ATTR_PURE__
int memcmp_PF(const void * lhs, uint_farptr_t rhs, std::size_t count)
{
	return std::memcmp(lhs, reinterpret_cast<const char *>(rhs), count);
}

void * memcpy_P(void * dest, const void * src, std::size_t count)
{
	return std::memcpy(dest, src, count);
}

void * memcpy_PF(void * dest, uint_farptr_t src, std::size_t count)
{
	return std::memcpy(dest, reinterpret_cast<const char *>(src), count);
}

//
// Extra character array manipulation functions
//

//const void * memrchr_P(const void * src, int ch, std::size_t count) = delete;

//void * memccpy_P(void * dest, const void * src, int ch, std::size_t count) = delete;

//__ATTR_PURE__ void * memmem_P(const void * src, std::size_t src_count, const void * target, std::size_t target_count) = delete;

//
// Extra miscellaneous function
//

//char * strsep_P(char *  * strptr, const char * delim) = delete;

