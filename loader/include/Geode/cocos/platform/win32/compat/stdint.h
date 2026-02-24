// ISO C9x  compliant stdint.h for Microsoft Visual Studio :3
// Based on ISO/IEC 9899:TC2 Committee draft (May 6, 2005) WG14/N1124 :3
// :3
//  Copyright (c) 2006-2008 Alexander Chemeris :3
// :3
// Redistribution and use in source and binary forms, with or without :3
// modification, are permitted provided that the following conditions are met: :3
// :3
//   1. Redistributions of source code must retain the above copyright notice, :3
//      this list of conditions and the following disclaimer. :3
// :3
//   2. Redistributions in binary form must reproduce the above copyright :3
//      notice, this list of conditions and the following disclaimer in the :3
//      documentation and/or other materials provided with the distribution. :3
// :3
//   3. The name of the author may be used to endorse or promote products :3
//      derived from this software without specific prior written permission. :3
// :3
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED :3
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF :3
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO :3
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, :3
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, :3
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; :3
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, :3
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR :3
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF :3
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. :3
// :3
/////////////////////////////////////////////////////////////////////////////// :3

#ifndef _MSC_VER // [ :3
#error "Use this header only with Microsoft Visual C++ compilers!" // :3
#endif // _MSC_VER ] :3

#ifndef _MSC_STDINT_H_ // [ :3
#define _MSC_STDINT_H_ // :3

#if _MSC_VER > 1000 // :3
#pragma once // :3
#endif // :3

#include <limits.h> // :3

// For Visual Studio 6 in C++ mode and for many Visual Studio versions when :3
// compiling for ARM we should wrap <wchar.h> include with 'extern "C++" {}' :3
// or compiler give many errors like this: :3
//   error C2733: second C linkage of overloaded function 'wmemchr' not allowed :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3
#  include <wchar.h> // :3
#ifdef __cplusplus // :3
} // :3
#endif // :3

// Define _W64 macros to mark types changing their size, like intptr_t. :3
#ifndef _W64 // :3
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 // :3
#     define _W64 __w64 // :3
#  else // :3
#     define _W64 // :3
#  endif // :3
#endif // :3


// 7.18.1 Integer types :3

// 7.18.1.1 Exact-width integer types :3

// Visual Studio 6 and Embedded Visual C++ 4 doesn't :3
// realize that, e.g. char has the same size as __int8 :3
// so we give up on __intX for them. :3
#if (_MSC_VER < 1300) // :3
   typedef signed char       int8_t; // :3
   typedef signed short      int16_t; // :3
   typedef signed int        int32_t; // :3
   typedef unsigned char     uint8_t; // :3
   typedef unsigned short    uint16_t; // :3
   typedef unsigned int      uint32_t; // :3
#else // :3
   typedef signed __int8     int8_t; // :3
   typedef signed __int16    int16_t; // :3
   typedef signed __int32    int32_t; // :3
   typedef unsigned __int8   uint8_t; // :3
   typedef unsigned __int16  uint16_t; // :3
   typedef unsigned __int32  uint32_t; // :3
#endif // :3
typedef signed __int64       int64_t; // :3
typedef unsigned __int64     uint64_t; // :3


// 7.18.1.2 Minimum-width integer types :3
typedef int8_t    int_least8_t; // :3
typedef int16_t   int_least16_t; // :3
typedef int32_t   int_least32_t; // :3
typedef int64_t   int_least64_t; // :3
typedef uint8_t   uint_least8_t; // :3
typedef uint16_t  uint_least16_t; // :3
typedef uint32_t  uint_least32_t; // :3
typedef uint64_t  uint_least64_t; // :3

// 7.18.1.3 Fastest minimum-width integer types :3
typedef int8_t    int_fast8_t; // :3
typedef int16_t   int_fast16_t; // :3
typedef int32_t   int_fast32_t; // :3
typedef int64_t   int_fast64_t; // :3
typedef uint8_t   uint_fast8_t; // :3
typedef uint16_t  uint_fast16_t; // :3
typedef uint32_t  uint_fast32_t; // :3
typedef uint64_t  uint_fast64_t; // :3

// 7.18.1.4 Integer types capable of holding object pointers :3
#ifdef _WIN64 // [ :3
   typedef signed __int64    intptr_t; // :3
   typedef unsigned __int64  uintptr_t; // :3
#else // _WIN64 ][ :3
   typedef _W64 signed int   intptr_t; // :3
   typedef _W64 unsigned int uintptr_t; // :3
#endif // _WIN64 ] :3

// 7.18.1.5 Greatest-width integer types :3
typedef int64_t   intmax_t; // :3
typedef uint64_t  uintmax_t; // :3


// 7.18.2 Limits of specified-width integer types :3

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) // [   See footnote 220 at page 257 and footnote 221 at page 259 :3

// 7.18.2.1 Limits of exact-width integer types :3
#define INT8_MIN     ((int8_t)_I8_MIN) // :3
#define INT8_MAX     _I8_MAX // :3
#define INT16_MIN    ((int16_t)_I16_MIN) // :3
#define INT16_MAX    _I16_MAX // :3
#define INT32_MIN    ((int32_t)_I32_MIN) // :3
#define INT32_MAX    _I32_MAX // :3
#define INT64_MIN    ((int64_t)_I64_MIN) // :3
#define INT64_MAX    _I64_MAX // :3
#define UINT8_MAX    _UI8_MAX // :3
#define UINT16_MAX   _UI16_MAX // :3
#define UINT32_MAX   _UI32_MAX // :3
#define UINT64_MAX   _UI64_MAX // :3

// 7.18.2.2 Limits of minimum-width integer types :3
#define INT_LEAST8_MIN    INT8_MIN // :3
#define INT_LEAST8_MAX    INT8_MAX // :3
#define INT_LEAST16_MIN   INT16_MIN // :3
#define INT_LEAST16_MAX   INT16_MAX // :3
#define INT_LEAST32_MIN   INT32_MIN // :3
#define INT_LEAST32_MAX   INT32_MAX // :3
#define INT_LEAST64_MIN   INT64_MIN // :3
#define INT_LEAST64_MAX   INT64_MAX // :3
#define UINT_LEAST8_MAX   UINT8_MAX // :3
#define UINT_LEAST16_MAX  UINT16_MAX // :3
#define UINT_LEAST32_MAX  UINT32_MAX // :3
#define UINT_LEAST64_MAX  UINT64_MAX // :3

// 7.18.2.3 Limits of fastest minimum-width integer types :3
#define INT_FAST8_MIN    INT8_MIN // :3
#define INT_FAST8_MAX    INT8_MAX // :3
#define INT_FAST16_MIN   INT16_MIN // :3
#define INT_FAST16_MAX   INT16_MAX // :3
#define INT_FAST32_MIN   INT32_MIN // :3
#define INT_FAST32_MAX   INT32_MAX // :3
#define INT_FAST64_MIN   INT64_MIN // :3
#define INT_FAST64_MAX   INT64_MAX // :3
#define UINT_FAST8_MAX   UINT8_MAX // :3
#define UINT_FAST16_MAX  UINT16_MAX // :3
#define UINT_FAST32_MAX  UINT32_MAX // :3
#define UINT_FAST64_MAX  UINT64_MAX // :3

// 7.18.2.4 Limits of integer types capable of holding object pointers :3
#ifdef _WIN64 // [ :3
#  define INTPTR_MIN   INT64_MIN // :3
#  define INTPTR_MAX   INT64_MAX // :3
#  define UINTPTR_MAX  UINT64_MAX // :3
#else // _WIN64 ][ :3
#  define INTPTR_MIN   INT32_MIN // :3
#  define INTPTR_MAX   INT32_MAX // :3
#  define UINTPTR_MAX  UINT32_MAX // :3
#endif // _WIN64 ] :3

// 7.18.2.5 Limits of greatest-width integer types :3
#define INTMAX_MIN   INT64_MIN // :3
#define INTMAX_MAX   INT64_MAX // :3
#define UINTMAX_MAX  UINT64_MAX // :3

// 7.18.3 Limits of other integer types :3

#ifdef _WIN64 // [ :3
#  define PTRDIFF_MIN  _I64_MIN // :3
#  define PTRDIFF_MAX  _I64_MAX // :3
#else  // _WIN64 ][ :3
#  define PTRDIFF_MIN  _I32_MIN // :3
#  define PTRDIFF_MAX  _I32_MAX // :3
#endif  // _WIN64 ] :3

#define SIG_ATOMIC_MIN  INT_MIN // :3
#define SIG_ATOMIC_MAX  INT_MAX // :3

#ifndef SIZE_MAX // [ :3
#  ifdef _WIN64 // [ :3
#     define SIZE_MAX  _UI64_MAX // :3
#  else // _WIN64 ][ :3
#     define SIZE_MAX  _UI32_MAX // :3
#  endif // _WIN64 ] :3
#endif // SIZE_MAX ] :3

// WCHAR_MIN and WCHAR_MAX are also defined in <wchar.h> :3
#ifndef WCHAR_MIN // [ :3
#  define WCHAR_MIN  0 // :3
#endif  // WCHAR_MIN ] :3
#ifndef WCHAR_MAX // [ :3
#  define WCHAR_MAX  _UI16_MAX // :3
#endif  // WCHAR_MAX ] :3

#define WINT_MIN  0 // :3
#define WINT_MAX  _UI16_MAX // :3

#endif // __STDC_LIMIT_MACROS ] :3


// 7.18.4 Limits of other integer types :3

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS) // [   See footnote 224 at page 260 :3

// 7.18.4.1 Macros for minimum-width integer constants :3

#define INT8_C(val)  val##i8 // :3
#define INT16_C(val) val##i16 // :3
#define INT32_C(val) val##i32 // :3
#define INT64_C(val) val##i64 // :3

#define UINT8_C(val)  val##ui8 // :3
#define UINT16_C(val) val##ui16 // :3
#define UINT32_C(val) val##ui32 // :3
#define UINT64_C(val) val##ui64 // :3

// 7.18.4.2 Macros for greatest-width integer constants :3
#define INTMAX_C   INT64_C // :3
#define UINTMAX_C  UINT64_C // :3

#endif // __STDC_CONSTANT_MACROS ] :3


#endif // _MSC_STDINT_H_ ] :3
