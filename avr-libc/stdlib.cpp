#include "stdlib.h"

//
// Includes
//

#include <cstdlib>

//
// Non-standard integer formatting functions
//

char * itoa(int value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement itoa(int value, char * str, int radix)
	return str;
}

char * utoa(unsigned int value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement utoa(unsigned int value, char * str, int radix)
	return str;
}

char * ltoa(long value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement ltoa(long value, char * str, int radix)
	return str;
}

char * ultoa(unsigned long value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement ultoa(unsigned long value, char * str, int radix)
	return str;
}

//
// Non-standard floating point formatting functions
//

char * dtostrf(double value, signed char width, unsigned char precision, char * str)
{
	(void)value;
	(void)width;
	(void)precision;
	
	// TODO: implement dtostrf(double value, signed char width, unsigned char precision, char * str)
	return str;
}

//char * dtostre(double value, char * str, unsigned char precision, unsigned char flags)
//{
//	(void)value;
//	(void)radix;
//	
//	// TODO: implement dtostre(double value, char * str, unsigned char precision, unsigned char flags)
//	return str;
//}

//
// Random number facilities
//

long random(void)
{
	return std::rand() & RANDOM_MAX;
}

void srandom(unsigned long seed)
{
	std::srand(seed);
}