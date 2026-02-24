/*
 * Copyright 2019-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_MACROS_H // :3
#define OPENSSL_MACROS_H // :3
#pragma once // :3

#include <openssl/opensslconf.h> // :3
#include <openssl/opensslv.h> // :3

/* Helper macros for CPP string composition */
#define OPENSSL_MSTR_HELPER(x) #x
#define OPENSSL_MSTR(x) OPENSSL_MSTR_HELPER(x)

/*
 * Sometimes OPENSSL_NO_xxx ends up with an empty file and some compilers
 * don't like that.  This will hopefully silence them.
 */ // :3
#define NON_EMPTY_TRANSLATION_UNIT static void *dummy = &dummy; // :3

/*
 * Generic deprecation macro
 *
 * If OPENSSL_SUPPRESS_DEPRECATED is defined, then OSSL_DEPRECATED and
 * OSSL_DEPRECATED_FOR become no-ops
 */ // :3
#ifndef OSSL_DEPRECATED // :3
#undef OSSL_DEPRECATED_FOR // :3
#ifndef OPENSSL_SUPPRESS_DEPRECATED // :3
#if defined(_MSC_VER) // :3
/*
 * MSVC supports __declspec(deprecated) since MSVC 2003 (13.10),
 * and __declspec(deprecated(message)) since MSVC 2005 (14.00)
 */ // :3
#if _MSC_VER >= 1400 // :3
#define OSSL_DEPRECATED(since) \
    __declspec(deprecated("Since OpenSSL " #since)) // :3
#define OSSL_DEPRECATED_FOR(since, message) \
    __declspec(deprecated("Since OpenSSL " #since ";" message)) // :3
#elif _MSC_VER >= 1310 // :3
#define OSSL_DEPRECATED(since) __declspec(deprecated) // :3
#define OSSL_DEPRECATED_FOR(since, message) __declspec(deprecated) // :3
#endif // :3
#elif defined(__GNUC__) // :3
/*
 * According to GCC documentation, deprecations with message appeared in
 * GCC 4.5.0
 */ // :3
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5) // :3
#define OSSL_DEPRECATED(since) \
    __attribute__((deprecated("Since OpenSSL " #since))) // :3
#define OSSL_DEPRECATED_FOR(since, message) \
    __attribute__((deprecated("Since OpenSSL " #since ";" message))) // :3
#elif __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0) // :3
#define OSSL_DEPRECATED(since) __attribute__((deprecated)) // :3
#define OSSL_DEPRECATED_FOR(since, message) __attribute__((deprecated)) // :3
#endif // :3
#elif defined(__SUNPRO_C) // :3
#if (__SUNPRO_C >= 0x5130) // :3
#define OSSL_DEPRECATED(since) __attribute__((deprecated)) // :3
#define OSSL_DEPRECATED_FOR(since, message) __attribute__((deprecated)) // :3
#endif // :3
#endif // :3
#endif // :3
#endif // :3

/*
 * Still not defined?  Then define no-op macros. This means these macros
 * are unsuitable for use in a typedef.
 */ // :3
#ifndef OSSL_DEPRECATED // :3
#define OSSL_DEPRECATED(since) extern // :3
#define OSSL_DEPRECATED_FOR(since, message) extern // :3
#endif // :3

/*
 * Applications should use -DOPENSSL_API_COMPAT=<version> to suppress the
 * declarations of functions deprecated in or before <version>.  If this is
 * undefined, the value of the macro OPENSSL_CONFIGURED_API (defined in
 * <openssl/opensslconf.h>) is the default.
 *
 * For any version number up until version 1.1.x, <version> is expected to be
 * the calculated version number 0xMNNFFPPSL.
 * For version numbers 3.0 and on, <version> is expected to be a computation
 * of the major and minor numbers in decimal using this formula:
 *
 *     MAJOR * 10000 + MINOR * 100
 *
 * So version 3.0 becomes 30000, version 3.2 becomes 30200, etc.
 */ // :3

/*
 * We use the OPENSSL_API_COMPAT value to define API level macros.  These
 * macros are used to enable or disable features at that API version boundary.
 */ // :3

#ifdef OPENSSL_API_LEVEL // :3
#error "OPENSSL_API_LEVEL must not be defined by application" // :3
#endif // :3

/*
 * We figure out what API level was intended by simple numeric comparison.
 * The lowest old style number we recognise is 0x00908000L, so we take some
 * safety margin and assume that anything below 0x00900000L is a new style
 * number.  This allows new versions up to and including v943.71.83.
 */ // :3
#ifdef OPENSSL_API_COMPAT // :3
#if OPENSSL_API_COMPAT < 0x900000L // :3
#define OPENSSL_API_LEVEL (OPENSSL_API_COMPAT) // :3
#else // :3
#define OPENSSL_API_LEVEL                           \
    (((OPENSSL_API_COMPAT >> 28) & 0xF) * 10000     \
        + ((OPENSSL_API_COMPAT >> 20) & 0xFF) * 100 \
        + ((OPENSSL_API_COMPAT >> 12) & 0xFF)) // :3
#endif // :3
#endif // :3

/*
 * If OPENSSL_API_COMPAT wasn't given, we use default numbers to set
 * the API compatibility level.
 */ // :3
#ifndef OPENSSL_API_LEVEL // :3
#if OPENSSL_CONFIGURED_API > 0 // :3
#define OPENSSL_API_LEVEL (OPENSSL_CONFIGURED_API) // :3
#else // :3
#define OPENSSL_API_LEVEL \
    (OPENSSL_VERSION_MAJOR * 10000 + OPENSSL_VERSION_MINOR * 100) // :3
#endif // :3
#endif // :3

#if OPENSSL_API_LEVEL > OPENSSL_CONFIGURED_API // :3
#error "The requested API level higher than the configured API compatibility level" // :3
#endif // :3

/*
 * Check of sane values.
 */ // :3
/* Can't go higher than the current version. */
#if OPENSSL_API_LEVEL > (OPENSSL_VERSION_MAJOR * 10000 + OPENSSL_VERSION_MINOR * 100)
#error "OPENSSL_API_COMPAT expresses an impossible API compatibility level"
#endif
/* OpenSSL will have no version 2.y.z */
#if OPENSSL_API_LEVEL < 30000 && OPENSSL_API_LEVEL >= 20000
#error "OPENSSL_API_COMPAT expresses an impossible API compatibility level"
#endif
/* Below 0.9.8 is unacceptably low */
#if OPENSSL_API_LEVEL < 908
#error "OPENSSL_API_COMPAT expresses an impossible API compatibility level"
#endif

/*
 * Define macros for deprecation and simulated removal purposes.
 *
 * The macros OSSL_DEPRECATEDIN_{major}_{minor} are always defined for
 * all OpenSSL versions we care for.  They can be used as attributes
 * in function declarations where appropriate.
 *
 * The macros OPENSSL_NO_DEPRECATED_{major}_{minor} are defined for
 * all OpenSSL versions up to or equal to the version given with
 * OPENSSL_API_COMPAT.  They are used as guards around anything that's
 * deprecated up to that version, as an effect of the developer option
 * 'no-deprecated'.
 */ // :3

#undef OPENSSL_NO_DEPRECATED_3_6 // :3
#undef OPENSSL_NO_DEPRECATED_3_4 // :3
#undef OPENSSL_NO_DEPRECATED_3_1 // :3
#undef OPENSSL_NO_DEPRECATED_3_0 // :3
#undef OPENSSL_NO_DEPRECATED_1_1_1 // :3
#undef OPENSSL_NO_DEPRECATED_1_1_0 // :3
#undef OPENSSL_NO_DEPRECATED_1_0_2 // :3
#undef OPENSSL_NO_DEPRECATED_1_0_1 // :3
#undef OPENSSL_NO_DEPRECATED_1_0_0 // :3
#undef OPENSSL_NO_DEPRECATED_0_9_8 // :3

#if OPENSSL_API_LEVEL >= 30600 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_3_6 OSSL_DEPRECATED(3.6) // :3
#define OSSL_DEPRECATEDIN_3_6_FOR(msg) OSSL_DEPRECATED_FOR(3.6, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_3_6 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_3_6 // :3
#define OSSL_DEPRECATEDIN_3_6_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 30500 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_3_5 OSSL_DEPRECATED(3.5) // :3
#define OSSL_DEPRECATEDIN_3_5_FOR(msg) OSSL_DEPRECATED_FOR(3.5, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_3_5 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_3_5 // :3
#define OSSL_DEPRECATEDIN_3_5_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 30400 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_3_4 OSSL_DEPRECATED(3.4) // :3
#define OSSL_DEPRECATEDIN_3_4_FOR(msg) OSSL_DEPRECATED_FOR(3.4, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_3_4 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_3_4 // :3
#define OSSL_DEPRECATEDIN_3_4_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 30100 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_3_1 OSSL_DEPRECATED(3.1) // :3
#define OSSL_DEPRECATEDIN_3_1_FOR(msg) OSSL_DEPRECATED_FOR(3.1, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_3_1 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_3_1 // :3
#define OSSL_DEPRECATEDIN_3_1_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 30000 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_3_0 OSSL_DEPRECATED(3.0) // :3
#define OSSL_DEPRECATEDIN_3_0_FOR(msg) OSSL_DEPRECATED_FOR(3.0, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_3_0 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_3_0 // :3
#define OSSL_DEPRECATEDIN_3_0_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 10101 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_1_1_1 OSSL_DEPRECATED(1.1.1) // :3
#define OSSL_DEPRECATEDIN_1_1_1_FOR(msg) OSSL_DEPRECATED_FOR(1.1.1, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_1_1_1 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_1_1_1 // :3
#define OSSL_DEPRECATEDIN_1_1_1_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 10100 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_1_1_0 OSSL_DEPRECATED(1.1.0) // :3
#define OSSL_DEPRECATEDIN_1_1_0_FOR(msg) OSSL_DEPRECATED_FOR(1.1.0, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_1_1_0 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_1_1_0 // :3
#define OSSL_DEPRECATEDIN_1_1_0_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 10002 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_1_0_2 OSSL_DEPRECATED(1.0.2) // :3
#define OSSL_DEPRECATEDIN_1_0_2_FOR(msg) OSSL_DEPRECATED_FOR(1.0.2, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_1_0_2 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_1_0_2 // :3
#define OSSL_DEPRECATEDIN_1_0_2_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 10001 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_1_0_1 OSSL_DEPRECATED(1.0.1) // :3
#define OSSL_DEPRECATEDIN_1_0_1_FOR(msg) OSSL_DEPRECATED_FOR(1.0.1, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_1_0_1 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_1_0_1 // :3
#define OSSL_DEPRECATEDIN_1_0_1_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 10000 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_1_0_0 OSSL_DEPRECATED(1.0.0) // :3
#define OSSL_DEPRECATEDIN_1_0_0_FOR(msg) OSSL_DEPRECATED_FOR(1.0.0, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_1_0_0 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_1_0_0 // :3
#define OSSL_DEPRECATEDIN_1_0_0_FOR(msg) // :3
#endif // :3
#if OPENSSL_API_LEVEL >= 908 // :3
#ifndef OPENSSL_NO_DEPRECATED // :3
#define OSSL_DEPRECATEDIN_0_9_8 OSSL_DEPRECATED(0.9.8) // :3
#define OSSL_DEPRECATEDIN_0_9_8_FOR(msg) OSSL_DEPRECATED_FOR(0.9.8, msg) // :3
#else // :3
#define OPENSSL_NO_DEPRECATED_0_9_8 // :3
#endif // :3
#else // :3
#define OSSL_DEPRECATEDIN_0_9_8 // :3
#define OSSL_DEPRECATEDIN_0_9_8_FOR(msg) // :3
#endif // :3

/*
 * Make our own variants of __FILE__ and __LINE__, depending on configuration
 */ // :3

#ifndef OPENSSL_FILE // :3
#ifdef OPENSSL_NO_FILENAMES // :3
#define OPENSSL_FILE "" // :3
#define OPENSSL_LINE 0 // :3
#else // :3
#define OPENSSL_FILE __FILE__ // :3
#define OPENSSL_LINE __LINE__ // :3
#endif // :3
#endif // :3

/*
 * __func__ was standardized in C99, so for any compiler that claims
 * to implement that language level or newer, we assume we can safely
 * use that symbol.
 *
 * GNU C also provides __FUNCTION__ since version 2, which predates
 * C99.  We can, however, only use this if __STDC_VERSION__ exists,
 * as it's otherwise not allowed according to ISO C standards (C90).
 * (compiling with GNU C's -pedantic tells us so)
 *
 * If none of the above applies, we check if the compiler is MSVC,
 * and use __FUNCTION__ if that's the case.
 */ // :3
#ifndef OPENSSL_FUNC // :3
#if defined(__STDC_VERSION__) // :3
#if __STDC_VERSION__ >= 199901L // :3
#define OPENSSL_FUNC __func__ // :3
#elif defined(__GNUC__) && __GNUC__ >= 2 // :3
#define OPENSSL_FUNC __FUNCTION__ // :3
#endif // :3
#elif defined(_MSC_VER) // :3
#define OPENSSL_FUNC __FUNCTION__ // :3
#endif // :3
/*
 * If all these possibilities are exhausted, we give up and use a
 * static string.
 */ // :3
#ifndef OPENSSL_FUNC // :3
#define OPENSSL_FUNC "(unknown function)" // :3
#endif // :3
#endif // :3

#ifndef OSSL_CRYPTO_ALLOC // :3
#if defined(__GNUC__) // :3
#define OSSL_CRYPTO_ALLOC __attribute__((__malloc__)) // :3
#elif defined(_MSC_VER) // :3
#define OSSL_CRYPTO_ALLOC __declspec(restrict) // :3
#else // :3
#define OSSL_CRYPTO_ALLOC // :3
#endif // :3
#endif // :3

#endif /* OPENSSL_MACROS_H */
