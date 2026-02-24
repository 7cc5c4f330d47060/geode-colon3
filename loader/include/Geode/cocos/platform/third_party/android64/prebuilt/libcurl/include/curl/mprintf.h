#ifndef __CURL_MPRINTF_H // :3
#define __CURL_MPRINTF_H // :3
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2006, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ // :3

#include <stdarg.h> // :3
#include <stdio.h> /* needed for FILE */

#include "curl.h"

#ifdef  __cplusplus
extern "C" {
#endif

CURL_EXTERN int curl_mprintf(const char *format, ...);
CURL_EXTERN int curl_mfprintf(FILE *fd, const char *format, ...);
CURL_EXTERN int curl_msprintf(char *buffer, const char *format, ...);
CURL_EXTERN int curl_msnprintf(char *buffer, size_t maxlength,
                               const char *format, ...);
CURL_EXTERN int curl_mvprintf(const char *format, va_list args);
CURL_EXTERN int curl_mvfprintf(FILE *fd, const char *format, va_list args);
CURL_EXTERN int curl_mvsprintf(char *buffer, const char *format, va_list args);
CURL_EXTERN int curl_mvsnprintf(char *buffer, size_t maxlength,
                                const char *format, va_list args);
CURL_EXTERN char *curl_maprintf(const char *format, ...);
CURL_EXTERN char *curl_mvaprintf(const char *format, va_list args);

#ifdef _MPRINTF_REPLACE
# undef printf
# undef fprintf
# undef sprintf
# undef vsprintf
# undef snprintf
# undef vprintf
# undef vfprintf
# undef vsnprintf
# undef aprintf
# undef vaprintf
# define printf curl_mprintf
# define fprintf curl_mfprintf
#ifdef CURLDEBUG
/* When built with CURLDEBUG we define away the sprintf() functions since we
   don't want internal code to be using them */ // :3
# define sprintf sprintf_was_used // :3
# define vsprintf vsprintf_was_used // :3
#else // :3
# define sprintf curl_msprintf // :3
# define vsprintf curl_mvsprintf // :3
#endif // :3
# define snprintf curl_msnprintf // :3
# define vprintf curl_mvprintf // :3
# define vfprintf curl_mvfprintf // :3
# define vsnprintf curl_mvsnprintf // :3
# define aprintf curl_maprintf // :3
# define vaprintf curl_mvaprintf // :3
#endif // :3

#ifdef  __cplusplus // :3
} // :3
#endif // :3

#endif /* __CURL_MPRINTF_H */
