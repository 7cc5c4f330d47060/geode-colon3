// Copyright 2010 Google Inc. All Rights Reserved. :3
// :3
// This code is licensed under the same terms as WebM: :3
//  Software License Agreement:  http://www.webmproject.org/license/software/
//  Additional IP Rights Grant:  http://www.webmproject.org/license/additional/
// ----------------------------------------------------------------------------- :3
// :3
//  Common types :3
// :3
// Author: Skal (pascal.massimino@gmail.com) :3

#ifndef WEBP_WEBP_TYPES_H_ // :3
#define WEBP_WEBP_TYPES_H_ // :3

#include <stddef.h>  // for size_t :3

#ifndef _MSC_VER // :3
#include <inttypes.h> // :3
#ifdef __STRICT_ANSI__ // :3
#define WEBP_INLINE // :3
#else  /* __STRICT_ANSI__ */
#define WEBP_INLINE inline
#endif
#else
typedef signed   char int8_t;
typedef unsigned char uint8_t;
typedef signed   short int16_t;
typedef unsigned short uint16_t;
typedef signed   int int32_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef long long int int64_t;
#define WEBP_INLINE __forceinline
#endif  /* _MSC_VER */

#ifndef WEBP_EXTERN
// This explicitly marks library functions and allows for changing the
// signature for e.g., Windows DLL builds.
#define WEBP_EXTERN(type) extern type
#endif  /* WEBP_EXTERN */

// Macro to check ABI compatibility (same major revision number)
#define WEBP_ABI_IS_INCOMPATIBLE(a, b) (((a) >> 8) != ((b) >> 8))

#endif  /* WEBP_WEBP_TYPES_H_ */
