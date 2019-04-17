#include "stdlib.h"

//
// Includes
//

#include <cstdlib>
#include <cstdio>

//
// Non-standard integer formatting functions
//

char * itoa(int value, char * str, int radix)
{
	if(radix != 10)
		return str;

	const int result = std::sprintf(str, "%d", value);

	if(result < 0)
		return str;

	const std::size_t size = static_cast<std::size_t>(result);

	return &str[size];
}

char * utoa(unsigned int value, char * str, int radix)
{
	const char * format = nullptr;

	switch(radix)
	{
		case 8:
			format = "%o";
			break;
		case 10:
			format = "%u";
			break;
		case 16:
			format = "%X";
			break;
	}

	if(format == nullptr)
		return str;

	const int result = std::sprintf(str, format, value);

	if(result < 0)
		return str;

	const std::size_t size = static_cast<std::size_t>(result);

	return &str[size];
}

char * ltoa(long value, char * str, int radix)
{
	if(radix != 10)
		return str;

	const int result = std::sprintf(str, "%ld", value);

	if(result < 0)
		return str;

	const std::size_t size = static_cast<std::size_t>(result);

	return &str[size];
}

char * ultoa(unsigned long value, char * str, int radix)
{
	const char * format = nullptr;

	switch(radix)
	{
		case 8:
			format = "%lo";
			break;
		case 10:
			format = "%lu";
			break;
		case 16:
			format = "%lX";
			break;
	}

	if(format == nullptr)
		return str;

	const int result = std::sprintf(str, format, value);

	if(result < 0)
		return str;

	const std::size_t size = static_cast<std::size_t>(result);

	return &str[size];
}

//
// Non-standard floating point formatting functions
//

char * dtostrf(double value, signed char width, unsigned char precision, char * str)
{
	static_cast<void>(value);
	static_cast<void>(width);
	static_cast<void>(precision);
	
	// TODO: implement dtostrf(double value, signed char width, unsigned char precision, char * str)
	return str;
}

//char * dtostre(double value, char * str, unsigned char precision, unsigned char flags)
//{
//	static_cast<void>(value);
//	static_cast<void>(radix);
//	
//	// TODO: implement dtostre(double value, char * str, unsigned char precision, unsigned char flags)
//	return str;
//}

//
// Random number facilities
//

long random()
{
	return std::rand() & RANDOM_MAX;
}

void srandom(unsigned long seed)
{
	std::srand(seed);
}