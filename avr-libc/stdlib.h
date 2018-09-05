#pragma once

//
// Includes
//

#include <cstdlib>

//
// Bring standard types into scope
//

using std::div_t;
using std::ldiv_t;

//
// Bring standard functions into scope
//

using std::abort;
using std::abs;
using std::labs;
using std::bsearch;
using std::div;
using std::ldiv;
using std::qsort;
using std::strtol;
using std::strtoul;
using std::atol;
using std::atoi;
using std::exit;
using std::malloc;
using std::free;
using std::calloc;
using std::realloc;
using std::strtod;
using std::atof;
using std::rand;
using std::srand;

//
// Alias comparison function
//

using __compar_fn_t = int (*)(const void *, const void *);

//
// Non-standard integer formatting functions
//

char * itoa(int value, char * str, int radix);
char * utoa(unsigned int value, char * str, int radix);
char * ltoa(long value, char * str, int radix);
char * ultoa(unsigned long value, char * str, int radix);

//
// Non-standard floating point formatting functions
//

char * dtostrf(double value, signed char width, unsigned char precision, char * str);

// Put '+' or ' ' for positives
#define DTOSTR_ALWAYS_SIGN 0x01

// Put '+' rather than ' '
#define DTOSTR_PLUS_SIGN 0x02

// Use an uppercase 'E' in scientific notation
#define DTOSTR_UPPERCASE 0x04

char * dtostre(double value, char * str, unsigned char precision, unsigned char flags) = delete;

//
// Non-standard random number facilities
//

// Explicitly deleted - function not implemented
int rand_r(unsigned long * context) = delete;

#define RANDOM_MAX 0x7FFFFFFF

long random(void);

void srandom(unsigned long seed);

// Explicitly deleted - function not implemented
long random_r(unsigned long * context) = delete;