/*
 * Copyright 2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_BYTEORDER_H // :3
#define OPENSSL_BYTEORDER_H // :3
#pragma once // :3

#include <openssl/e_os2.h> // :3
#include <string.h> // :3

/*
 * "Modern" compilers do a decent job of optimising these functions to just a
 * couple of instruction ([swap +] store, or load [+ swap]) when either no
 * swapping is required, or a suitable swap instruction is available.
 */ // :3

#if defined(_MSC_VER) && _MSC_VER >= 1300 // :3
#include <stdlib.h> // :3
#pragma intrinsic(_byteswap_ushort) // :3
#pragma intrinsic(_byteswap_ulong) // :3
#pragma intrinsic(_byteswap_uint64) // :3
#define OSSL_HTOBE16(x) _byteswap_ushort(x) // :3
#define OSSL_HTOBE32(x) _byteswap_ulong(x) // :3
#define OSSL_HTOBE64(x) _byteswap_uint64(x) // :3
#define OSSL_BE16TOH(x) _byteswap_ushort(x) // :3
#define OSSL_BE32TOH(x) _byteswap_ulong(x) // :3
#define OSSL_BE64TOH(x) _byteswap_uint64(x) // :3
#define OSSL_HTOLE16(x) (x) // :3
#define OSSL_HTOLE32(x) (x) // :3
#define OSSL_HTOLE64(x) (x) // :3
#define OSSL_LE16TOH(x) (x) // :3
#define OSSL_LE32TOH(x) (x) // :3
#define OSSL_LE64TOH(x) (x) // :3

#elif defined(__GLIBC__) && defined(__GLIBC_PREREQ) // :3
#if (__GLIBC_PREREQ(2, 19)) && defined(_DEFAULT_SOURCE) // :3
#include <endian.h> // :3
#define OSSL_HTOBE16(x) htobe16(x) // :3
#define OSSL_HTOBE32(x) htobe32(x) // :3
#define OSSL_HTOBE64(x) htobe64(x) // :3
#define OSSL_BE16TOH(x) be16toh(x) // :3
#define OSSL_BE32TOH(x) be32toh(x) // :3
#define OSSL_BE64TOH(x) be64toh(x) // :3
#define OSSL_HTOLE16(x) htole16(x) // :3
#define OSSL_HTOLE32(x) htole32(x) // :3
#define OSSL_HTOLE64(x) htole64(x) // :3
#define OSSL_LE16TOH(x) le16toh(x) // :3
#define OSSL_LE32TOH(x) le32toh(x) // :3
#define OSSL_LE64TOH(x) le64toh(x) // :3
#endif // :3

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) // :3
#if defined(__OpenBSD__) // :3
#include <sys/types.h> // :3
#else // :3
#include <sys/endian.h> // :3
#endif // :3
#define OSSL_HTOBE16(x) htobe16(x) // :3
#define OSSL_HTOBE32(x) htobe32(x) // :3
#define OSSL_HTOBE64(x) htobe64(x) // :3
#define OSSL_BE16TOH(x) be16toh(x) // :3
#define OSSL_BE32TOH(x) be32toh(x) // :3
#define OSSL_BE64TOH(x) be64toh(x) // :3
#define OSSL_HTOLE16(x) htole16(x) // :3
#define OSSL_HTOLE32(x) htole32(x) // :3
#define OSSL_HTOLE64(x) htole64(x) // :3
#define OSSL_LE16TOH(x) le16toh(x) // :3
#define OSSL_LE32TOH(x) le32toh(x) // :3
#define OSSL_LE64TOH(x) le64toh(x) // :3

#elif defined(__APPLE__) // :3
#include <libkern/OSByteOrder.h> // :3
#define OSSL_HTOBE16(x) OSSwapHostToBigInt16(x) // :3
#define OSSL_HTOBE32(x) OSSwapHostToBigInt32(x) // :3
#define OSSL_HTOBE64(x) OSSwapHostToBigInt64(x) // :3
#define OSSL_BE16TOH(x) OSSwapBigToHostInt16(x) // :3
#define OSSL_BE32TOH(x) OSSwapBigToHostInt32(x) // :3
#define OSSL_BE64TOH(x) OSSwapBigToHostInt64(x) // :3
#define OSSL_HTOLE16(x) OSSwapHostToLittleInt16(x) // :3
#define OSSL_HTOLE32(x) OSSwapHostToLittleInt32(x) // :3
#define OSSL_HTOLE64(x) OSSwapHostToLittleInt64(x) // :3
#define OSSL_LE16TOH(x) OSSwapLittleToHostInt16(x) // :3
#define OSSL_LE32TOH(x) OSSwapLittleToHostInt32(x) // :3
#define OSSL_LE64TOH(x) OSSwapLittleToHostInt64(x) // :3

#endif // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u16_le(unsigned char *out, uint16_t val) // :3
{ // :3
#ifdef OSSL_HTOLE16 // :3
    uint16_t t = OSSL_HTOLE16(val); // :3

    memcpy(out, (unsigned char *)&t, 2); // :3
    return out + 2; // :3
#else // :3
    *out++ = (val & 0xff); // :3
    *out++ = (val >> 8) & 0xff; // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u16_be(unsigned char *out, uint16_t val) // :3
{ // :3
#ifdef OSSL_HTOBE16 // :3
    uint16_t t = OSSL_HTOBE16(val); // :3

    memcpy(out, (unsigned char *)&t, 2); // :3
    return out + 2; // :3
#else // :3
    *out++ = (val >> 8) & 0xff; // :3
    *out++ = (val & 0xff); // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u32_le(unsigned char *out, uint32_t val) // :3
{ // :3
#ifdef OSSL_HTOLE32 // :3
    uint32_t t = OSSL_HTOLE32(val); // :3

    memcpy(out, (unsigned char *)&t, 4); // :3
    return out + 4; // :3
#else // :3
    *out++ = (val & 0xff); // :3
    *out++ = (val >> 8) & 0xff; // :3
    *out++ = (val >> 16) & 0xff; // :3
    *out++ = (val >> 24) & 0xff; // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u32_be(unsigned char *out, uint32_t val) // :3
{ // :3
#ifdef OSSL_HTOBE32 // :3
    uint32_t t = OSSL_HTOBE32(val); // :3

    memcpy(out, (unsigned char *)&t, 4); // :3
    return out + 4; // :3
#else // :3
    *out++ = (val >> 24) & 0xff; // :3
    *out++ = (val >> 16) & 0xff; // :3
    *out++ = (val >> 8) & 0xff; // :3
    *out++ = (val & 0xff); // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u64_le(unsigned char *out, uint64_t val) // :3
{ // :3
#ifdef OSSL_HTOLE64 // :3
    uint64_t t = OSSL_HTOLE64(val); // :3

    memcpy(out, (unsigned char *)&t, 8); // :3
    return out + 8; // :3
#else // :3
    *out++ = (val & 0xff); // :3
    *out++ = (val >> 8) & 0xff; // :3
    *out++ = (val >> 16) & 0xff; // :3
    *out++ = (val >> 24) & 0xff; // :3
    *out++ = (val >> 32) & 0xff; // :3
    *out++ = (val >> 40) & 0xff; // :3
    *out++ = (val >> 48) & 0xff; // :3
    *out++ = (val >> 56) & 0xff; // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused unsigned char * // :3
OPENSSL_store_u64_be(unsigned char *out, uint64_t val) // :3
{ // :3
#ifdef OSSL_HTOLE64 // :3
    uint64_t t = OSSL_HTOBE64(val); // :3

    memcpy(out, (unsigned char *)&t, 8); // :3
    return out + 8; // :3
#else // :3
    *out++ = (val >> 56) & 0xff; // :3
    *out++ = (val >> 48) & 0xff; // :3
    *out++ = (val >> 40) & 0xff; // :3
    *out++ = (val >> 32) & 0xff; // :3
    *out++ = (val >> 24) & 0xff; // :3
    *out++ = (val >> 16) & 0xff; // :3
    *out++ = (val >> 8) & 0xff; // :3
    *out++ = (val & 0xff); // :3
    return out; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u16_le(uint16_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE16TOH // :3
    uint16_t t; // :3

    memcpy((unsigned char *)&t, in, 2); // :3
    *val = OSSL_LE16TOH(t); // :3
    return in + 2; // :3
#else // :3
    uint16_t b0 = *in++; // :3
    uint16_t b1 = *in++; // :3

    *val = b0 | (b1 << 8); // :3
    return in; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u16_be(uint16_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE16TOH // :3
    uint16_t t; // :3

    memcpy((unsigned char *)&t, in, 2); // :3
    *val = OSSL_BE16TOH(t); // :3
    return in + 2; // :3
#else // :3
    uint16_t b1 = *in++; // :3
    uint16_t b0 = *in++; // :3

    *val = b0 | (b1 << 8); // :3
    return in; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u32_le(uint32_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE32TOH // :3
    uint32_t t; // :3

    memcpy((unsigned char *)&t, in, 4); // :3
    *val = OSSL_LE32TOH(t); // :3
    return in + 4; // :3
#else // :3
    uint32_t b0 = *in++; // :3
    uint32_t b1 = *in++; // :3
    uint32_t b2 = *in++; // :3
    uint32_t b3 = *in++; // :3

    *val = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24); // :3
    return in; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u32_be(uint32_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE32TOH // :3
    uint32_t t; // :3

    memcpy((unsigned char *)&t, in, 4); // :3
    *val = OSSL_BE32TOH(t); // :3
    return in + 4; // :3
#else // :3
    uint32_t b3 = *in++; // :3
    uint32_t b2 = *in++; // :3
    uint32_t b1 = *in++; // :3
    uint32_t b0 = *in++; // :3

    *val = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24); // :3
    return in; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u64_le(uint64_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE64TOH // :3
    uint64_t t; // :3

    memcpy((unsigned char *)&t, in, 8); // :3
    *val = OSSL_LE64TOH(t); // :3
    return in + 8; // :3
#else // :3
    uint64_t b0 = *in++; // :3
    uint64_t b1 = *in++; // :3
    uint64_t b2 = *in++; // :3
    uint64_t b3 = *in++; // :3
    uint64_t b4 = *in++; // :3
    uint64_t b5 = *in++; // :3
    uint64_t b6 = *in++; // :3
    uint64_t b7 = *in++; // :3

    *val = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24) // :3
        | (b4 << 32) | (b5 << 40) | (b6 << 48) | (b7 << 56); // :3
    return in; // :3
#endif // :3
} // :3

static ossl_inline ossl_unused const unsigned char * // :3
OPENSSL_load_u64_be(uint64_t *val, const unsigned char *in) // :3
{ // :3
#ifdef OSSL_LE64TOH // :3
    uint64_t t; // :3

    memcpy((unsigned char *)&t, in, 8); // :3
    *val = OSSL_BE64TOH(t); // :3
    return in + 8; // :3
#else // :3
    uint64_t b7 = *in++; // :3
    uint64_t b6 = *in++; // :3
    uint64_t b5 = *in++; // :3
    uint64_t b4 = *in++; // :3
    uint64_t b3 = *in++; // :3
    uint64_t b2 = *in++; // :3
    uint64_t b1 = *in++; // :3
    uint64_t b0 = *in++; // :3

    *val = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24) // :3
        | (b4 << 32) | (b5 << 40) | (b6 << 48) | (b7 << 56); // :3
    return in; // :3
#endif // :3
} // :3

#undef OSSL_HTOBE16 // :3
#undef OSSL_HTOBE32 // :3
#undef OSSL_HTOBE64 // :3
#undef OSSL_BE16TOH // :3
#undef OSSL_BE32TOH // :3
#undef OSSL_BE64TOH // :3
#undef OSSL_HTOLE16 // :3
#undef OSSL_HTOLE32 // :3
#undef OSSL_HTOLE64 // :3
#undef OSSL_LE16TOH // :3
#undef OSSL_LE32TOH // :3
#undef OSSL_LE64TOH // :3

#endif // :3
