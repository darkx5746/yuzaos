#pragma once

#define __need_wint_t
#define __need_wchar_t
#include <size_t.h>
/* 7.18.1.1  Exact-width integer types */
typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned int   uint32_t;
typedef long long  int64_t;
typedef unsigned long long   uint64_t;
typedef int64_t register_t;


/* 7.18.1.2  Minimum-width integer types */
typedef signed char int_least8_t;
typedef unsigned char   uint_least8_t;
typedef short  int_least16_t;
typedef unsigned short  uint_least16_t;
typedef int  int_least32_t;
typedef unsigned   uint_least32_t;
typedef long long  int_least64_t;
typedef unsigned long long   uint_least64_t;

/*  7.18.1.3  Fastest minimum-width integer types 
 *  Not actually guaranteed to be fastest for all purposes
 *  Here we use the exact-width types for 8 and 16-bit ints. 
 */
typedef char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short  int_fast16_t;
typedef unsigned short  uint_fast16_t;
typedef int  int_fast32_t;
typedef unsigned  int  uint_fast32_t;
typedef long long  int_fast64_t;
typedef unsigned long long   uint_fast64_t;

/* 7.18.1.4  Integer types capable of holding object pointers */
typedef int intptr_t;
typedef unsigned uintptr_t;

/* 7.18.1.5  Greatest-width integer types */
typedef long long  intmax_t;
typedef unsigned long long   uintmax_t;

/* 7.18.2  Limits of specified-width integer types */
//#if !defined ( __cplusplus) || defined (__STDC_LIMIT_MACROS)

/* 7.18.2.1  Limits of exact-width integer types */
#define INT8_MIN (-128) 
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483647 - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807LL

#define UINT8_MAX 0xff /* 255U */
#define UINT16_MAX 0xffff /* 65535U */
#define UINT32_MAX 0xffffffff  /* 4294967295U */
#define UINT64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */

/* 7.18.2.2  Limits of minimum-width integer types */
#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/* 7.18.2.3  Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

/* 7.18.2.4  Limits of integer types capable of holding
    object pointers */ 
#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

/* 7.18.2.5  Limits of greatest-width integer types */
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/* 7.18.3  Limits of other integer types */
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#define SIZE_MAX UINT32_MAX

#ifndef WCHAR_MIN  /* also in wchar.h */ 
#define WCHAR_MIN 0
#define WCHAR_MAX ((wchar_t)-1) /* UINT16_MAX */
#endif

#if defined(SKYOS32)
#define PACKED 
#define ATTRIBUTE_WEAK __declspec(selectany)
#else
#define PACKED __attribute__ ((__packed__))

#define ATTRIBUTE_WEAK __attribute__((weak))
#endif

/*
 * wint_t is unsigned short for compatibility with MS runtime
 */
#define WINT_MIN 0
#define WINT_MAX ((wint_t)-1) /* UINT16_MAX */

//#endif /* !defined ( __cplusplus) || defined __STDC_LIMIT_MACROS */


/* 7.18.4  Macros for integer constants */
#if !defined ( __cplusplus) || defined (__STDC_CONSTANT_MACROS)

/* 7.18.4.1  Macros for minimum-width integer constants */

#define INT8_C(val) ((int8_t) + (val))
#define UINT8_C(val) ((uint8_t) + (val##U))
#define INT16_C(val) ((int16_t) + (val))
#define UINT16_C(val) ((uint16_t) + (val##U))

#define INT32_C(val) val##L
#define UINT32_C(val) val##UL
#define INT64_C(val) val##LL
#define UINT64_C(val) val##ULL

/* 7.18.4.2  Macros for greatest-width integer constants */
#define INTMAX_C(val)  INT64_C(val)
#define UINTMAX_C(val) UINT64_C(val)

#endif  /* !defined ( __cplusplus) || defined __STDC_CONSTANT_MACROS */

//! Set bit
inline static uint8_t SetBit(uint8_t word, uint8_t mask) {
	return word | mask;
}

/*
//! Clears bit
static uint8_t ClrBit(uint8_t word, uint8_t mask)
{
	return word & (~mask);
}

//! Assigns the bit defined in mask to a value
static uint8_t AssignBit(uint8_t word, uint8_t mask, uint8_t value)
{
	return value ? SetBit(word, mask) : ClrBit(word, mask);
}*/
