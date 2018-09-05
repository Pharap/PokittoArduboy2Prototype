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
	
	// TODO: implement this;
	return str;
}

char * utoa(unsigned int value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement this;
	return str;
}

char * ltoa(long value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement this;
	return str;
}

char * ultoa(unsigned long value, char * str, int radix)
{
	(void)value;
	(void)radix;
	
	// TODO: implement this;
	return str;
}

//
// Non-standard floating point formatting functions
//

char * dtostrf(double value, signed char width, unsigned char prec, char * str)
{
	(void)value;
	(void)width;
	(void)prec;
	
	// TODO: implement this;
	return s;
}

//char * dtostre(double value, char * s, unsigned char prec, unsigned char flags)
//{
//	(void)value;
//	(void)radix;
//	
//	// TODO: implement this;
//	return s;
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