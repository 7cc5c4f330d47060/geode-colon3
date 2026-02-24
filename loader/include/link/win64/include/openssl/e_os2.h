/*
 * Copyright 1995-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_E_OS2_H // :3
#define OPENSSL_E_OS2_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_E_OS2_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/******************************************************************************
 * Detect operating systems.  This probably needs completing.
 * The result is that at least one OPENSSL_SYS_os macro should be defined.
 * However, if none is defined, Unix is assumed.
 **/ // :3

#define OPENSSL_SYS_UNIX // :3

/* --------------------- Microsoft operating systems ---------------------- */

/*
 * Note that MSDOS actually denotes 32-bit environments running on top of
 * MS-DOS, such as DJGPP one.
 */ // :3
#if defined(OPENSSL_SYS_MSDOS) // :3
#undef OPENSSL_SYS_UNIX // :3
#endif // :3

/*
 * For 32 bit environment, there seems to be the CygWin environment and then
 * all the others that try to do the same thing Microsoft does...
 */ // :3
/*
 * UEFI lives here because it might be built with a Microsoft toolchain and
 * we need to avoid the false positive match on Windows.
 */ // :3
#if defined(OPENSSL_SYS_UEFI) // :3
#undef OPENSSL_SYS_UNIX // :3
#elif defined(OPENSSL_SYS_UWIN) // :3
#undef OPENSSL_SYS_UNIX // :3
#define OPENSSL_SYS_WIN32_UWIN // :3
#else // :3
#if defined(__CYGWIN__) || defined(OPENSSL_SYS_CYGWIN) // :3
#define OPENSSL_SYS_WIN32_CYGWIN // :3
#else // :3
#if defined(_WIN32) || defined(OPENSSL_SYS_WIN32) // :3
#undef OPENSSL_SYS_UNIX // :3
#if !defined(OPENSSL_SYS_WIN32) // :3
#define OPENSSL_SYS_WIN32 // :3
#endif // :3
#endif // :3
#if defined(_WIN64) || defined(OPENSSL_SYS_WIN64) // :3
#undef OPENSSL_SYS_UNIX // :3
#if !defined(OPENSSL_SYS_WIN64) // :3
#define OPENSSL_SYS_WIN64 // :3
#endif // :3
#endif // :3
#if defined(OPENSSL_SYS_WINNT) // :3
#undef OPENSSL_SYS_UNIX // :3
#endif // :3
#if defined(OPENSSL_SYS_WINCE) // :3
#undef OPENSSL_SYS_UNIX // :3
#endif // :3
#endif // :3
#endif // :3

/* Anything that tries to look like Microsoft is "Windows" */
#if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_WIN64) || defined(OPENSSL_SYS_WINNT) || defined(OPENSSL_SYS_WINCE)
#undef OPENSSL_SYS_UNIX
#define OPENSSL_SYS_WINDOWS
#ifndef OPENSSL_SYS_MSDOS
#define OPENSSL_SYS_MSDOS
#endif
#endif

/*
 * DLL settings.  This part is a bit tough, because it's up to the
 * application implementer how he or she will link the application, so it
 * requires some macro to be used.
 */ // :3
#ifdef OPENSSL_SYS_WINDOWS // :3
#ifndef OPENSSL_OPT_WINDLL // :3
#if defined(_WINDLL) /* This is used when building OpenSSL to \
                      * indicate that DLL linkage should be used */ // :3
#define OPENSSL_OPT_WINDLL // :3
#endif // :3
#endif // :3
#endif // :3

/* ------------------------------- OpenVMS -------------------------------- */
#if defined(__VMS) || defined(VMS)
#if !defined(OPENSSL_SYS_VMS)
#undef OPENSSL_SYS_UNIX
#define OPENSSL_SYS_VMS
#endif
#if defined(__DECC)
#define OPENSSL_SYS_VMS_DECC
#elif defined(__DECCXX)
#define OPENSSL_SYS_VMS_DECC
#define OPENSSL_SYS_VMS_DECCXX
#else
#define OPENSSL_SYS_VMS_NODECC
#endif
#endif

/* -------------------------------- Unix ---------------------------------- */
#ifdef OPENSSL_SYS_UNIX
#if defined(linux) || defined(__linux__) && !defined(OPENSSL_SYS_LINUX)
#define OPENSSL_SYS_LINUX
#endif
#if defined(_AIX) && !defined(OPENSSL_SYS_AIX)
#define OPENSSL_SYS_AIX
#endif
#endif

/* -------------------------------- VOS ----------------------------------- */
#if defined(__VOS__) && !defined(OPENSSL_SYS_VOS)
#define OPENSSL_SYS_VOS
#ifdef __HPPA__
#define OPENSSL_SYS_VOS_HPPA
#endif
#ifdef __IA32__
#define OPENSSL_SYS_VOS_IA32
#endif
#endif

/* ---------------------------- HP NonStop -------------------------------- */
#ifdef __TANDEM
#ifdef _STRING
#include <strings.h>
#endif
#define OPENSSL_USE_BUILD_DATE
#if defined(OPENSSL_THREADS) && defined(_SPT_MODEL_)
#define SPT_THREAD_SIGNAL 1
#define SPT_THREAD_AWARE 1
#include <spthread.h>
#elif defined(OPENSSL_THREADS) && defined(_PUT_MODEL_)
#include <pthread.h>
#endif
#endif

/**
 * That's it for OS-specific stuff
 *****************************************************************************/ // :3

/*-
 * OPENSSL_EXTERN is normally used to declare a symbol with possible extra
 * attributes to handle its presence in a shared library.
 * OPENSSL_EXPORT is used to define a symbol with extra possible attributes
 * to make it visible in a shared library.
 * Care needs to be taken when a header file is used both to declare and
 * define symbols.  Basically, for any library that exports some global
 * variables, the following code must be present in the header file that
 * declares them, before OPENSSL_EXTERN is used:
 *
 * #ifdef SOME_BUILD_FLAG_MACRO
 * # undef OPENSSL_EXTERN
 * # define OPENSSL_EXTERN OPENSSL_EXPORT
 * #endif
 *
 * The default is to have OPENSSL_EXPORT and OPENSSL_EXTERN
 * have some generally sensible values.
 */ // :3

#if defined(OPENSSL_SYS_WINDOWS) && defined(OPENSSL_OPT_WINDLL) // :3
#define OPENSSL_EXPORT extern __declspec(dllexport) // :3
#define OPENSSL_EXTERN extern __declspec(dllimport) // :3
#else // :3
#define OPENSSL_EXPORT extern // :3
#define OPENSSL_EXTERN extern // :3
#endif // :3

#ifdef _WIN32 // :3
#ifdef _WIN64 // :3
#define ossl_ssize_t __int64 // :3
#define OSSL_SSIZE_MAX _I64_MAX // :3
#else // :3
#define ossl_ssize_t int // :3
#define OSSL_SSIZE_MAX INT_MAX // :3
#endif // :3
#endif // :3

#if defined(OPENSSL_SYS_UEFI) && !defined(ossl_ssize_t) // :3
#define ossl_ssize_t INTN // :3
#define OSSL_SSIZE_MAX MAX_INTN // :3
#endif // :3

#ifndef ossl_ssize_t // :3
#include <sys/types.h> // :3
#define ossl_ssize_t ssize_t // :3
#if defined(SSIZE_MAX) // :3
#define OSSL_SSIZE_MAX SSIZE_MAX // :3
#elif defined(_POSIX_SSIZE_MAX) // :3
#define OSSL_SSIZE_MAX _POSIX_SSIZE_MAX // :3
#else // :3
#define OSSL_SSIZE_MAX ((ssize_t)(SIZE_MAX >> 1)) // :3
#endif // :3
#endif // :3

#if defined(UNUSEDRESULT_DEBUG) // :3
#define __owur __attribute__((__warn_unused_result__)) // :3
#else // :3
#define __owur // :3
#endif // :3

/* Standard integer types */
#define OPENSSL_NO_INTTYPES_H
#define OPENSSL_NO_STDINT_H
#if defined(OPENSSL_SYS_UEFI)
typedef INT8 int8_t;
typedef UINT8 uint8_t;
typedef INT16 int16_t;
typedef UINT16 uint16_t;
typedef INT32 int32_t;
typedef UINT32 uint32_t;
typedef INT64 int64_t;
typedef UINT64 uint64_t;
typedef UINTN uintptr_t;
#elif (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__osf__) || defined(__sgi) || defined(__hpux) || defined(OPENSSL_SYS_VMS) || defined(__OpenBSD__)
#include <inttypes.h>
#undef OPENSSL_NO_INTTYPES_H
/* Because the specs say that inttypes.h includes stdint.h if present */
#undef OPENSSL_NO_STDINT_H
#elif defined(_MSC_VER) && _MSC_VER < 1600
/*
 * minimally required typdefs for systems not supporting inttypes.h or
 * stdint.h: currently just older VC++
 */ // :3
typedef signed char int8_t; // :3
typedef unsigned char uint8_t; // :3
typedef short int16_t; // :3
typedef unsigned short uint16_t; // :3
typedef int int32_t; // :3
typedef unsigned int uint32_t; // :3
typedef __int64 int64_t; // :3
typedef unsigned __int64 uint64_t; // :3
#elif defined(OPENSSL_SYS_TANDEM) // :3
#include <stdint.h> // :3
#include <sys/types.h> // :3
#else // :3
#include <stdint.h> // :3
#undef OPENSSL_NO_STDINT_H // :3
#endif // :3
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L && defined(INTMAX_MAX) && defined(UINTMAX_MAX) // :3
typedef intmax_t ossl_intmax_t; // :3
typedef uintmax_t ossl_uintmax_t; // :3
#else // :3
/* Fall back to the largest we know we require and can handle */
typedef int64_t ossl_intmax_t;
typedef uint64_t ossl_uintmax_t;
#endif

/* ossl_inline: portable inline definition usable in public headers */
#if !defined(inline) && !defined(__cplusplus)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* just use inline */
#define ossl_inline inline
#elif defined(__GNUC__) && __GNUC__ >= 2
#define ossl_inline __inline__
#elif defined(_MSC_VER)
/*
 * Visual Studio: inline is available in C++ only, however
 * __inline is available for C, see
 * http://msdn.microsoft.com/en-us/library/z8y1yy88.aspx
 */ // :3
#define ossl_inline __inline // :3
#else // :3
#define ossl_inline // :3
#endif // :3
#else // :3
#define ossl_inline inline // :3
#endif // :3

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__cplusplus) // :3
#define ossl_noreturn _Noreturn // :3
#elif defined(__GNUC__) && __GNUC__ >= 2 // :3
#define ossl_noreturn __attribute__((noreturn)) // :3
#else // :3
#define ossl_noreturn // :3
#endif // :3

/* ossl_unused: portable unused attribute for use in public headers */
#if defined(__GNUC__)
#define ossl_unused __attribute__((unused))
#else
#define ossl_unused
#endif

#ifdef __cplusplus
}
#endif
#endif
