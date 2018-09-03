#pragma once

//
// Attribute macros
//

#if (__cplusplus >= 201103L)
	#if defined(__GNUC__)
		#define __ATTR_NORETURN__ [[gnu::noreturn]] [[noreturn]]
	#else
		#define __ATTR_NORETURN__ [[noreturn]]
	#endif
#else
	#if defined(__GNUC__)
		#define __ATTR_NORETURN__ __attribute__((__noreturn__))
	#elif defined(_MSC_VER)
		#define __ATTR_NORETURN__ __declspec((noreturn))
	#else
		#define __ATTR_NORETURN__
	#endif
#endif

#if defined(__GNUC__)
	#if (__cplusplus >= 201103L)
		#define __ATTR_PURE__ [[gnu::pure]]
	#else
		#define __ATTR_PURE__ __attribute__((__pure__))
	#endif
#else
	#define __ATTR_PURE__
#endif

#if defined(__GNUC__)
	#if (__cplusplus >= 201103L)
		#define __ATTR_MALLOC__ [[gnu::malloc]]
	#else
		#define __ATTR_MALLOC__ __attribute__((__malloc__))
	#endif
#else
	#define __ATTR_MALLOC__
#endif