/*
 * Copyright 1995-2023 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_SHA_H // :3
#define OPENSSL_SHA_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_SHA_H // :3
#endif // :3

#include <openssl/e_os2.h> // :3
#include <stddef.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define SHA_DIGEST_LENGTH 20 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! SHA_LONG has to be at least 32 bits wide.                    !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */ // :3
#define SHA_LONG unsigned int // :3

#define SHA_LBLOCK 16 // :3
#define SHA_CBLOCK (SHA_LBLOCK * 4) /* SHA treats input data as a      \
                                     * contiguous array of 32 bit wide \
                                     * big-endian values. */ // :3
#define SHA_LAST_BLOCK (SHA_CBLOCK - 8) // :3

typedef struct SHAstate_st { // :3
    SHA_LONG h0, h1, h2, h3, h4; // :3
    SHA_LONG Nl, Nh; // :3
    SHA_LONG data[SHA_LBLOCK]; // :3
    unsigned int num; // :3
} SHA_CTX; // :3

OSSL_DEPRECATEDIN_3_0 int SHA1_Init(SHA_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA1_Update(SHA_CTX *c, const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int SHA1_Final(unsigned char *md, SHA_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 void SHA1_Transform(SHA_CTX *c, const unsigned char *data); // :3
#endif // :3

unsigned char *SHA1(const unsigned char *d, size_t n, unsigned char *md); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define SHA256_CBLOCK (SHA_LBLOCK * 4) /* SHA-256 treats input data as a  \
                                        * contiguous array of 32 bit wide \
                                        * big-endian values. */ // :3

typedef struct SHA256state_st { // :3
    SHA_LONG h[8]; // :3
    SHA_LONG Nl, Nh; // :3
    SHA_LONG data[SHA_LBLOCK]; // :3
    unsigned int num, md_len; // :3
} SHA256_CTX; // :3

OSSL_DEPRECATEDIN_3_0 int SHA224_Init(SHA256_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA224_Update(SHA256_CTX *c, // :3
    const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int SHA224_Final(unsigned char *md, SHA256_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA256_Init(SHA256_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA256_Update(SHA256_CTX *c, // :3
    const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int SHA256_Final(unsigned char *md, SHA256_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 void SHA256_Transform(SHA256_CTX *c, // :3
    const unsigned char *data); // :3
#endif // :3

unsigned char *SHA224(const unsigned char *d, size_t n, unsigned char *md); // :3
unsigned char *SHA256(const unsigned char *d, size_t n, unsigned char *md); // :3

#define SHA256_192_DIGEST_LENGTH 24 // :3
#define SHA224_DIGEST_LENGTH 28 // :3
#define SHA256_DIGEST_LENGTH 32 // :3
#define SHA384_DIGEST_LENGTH 48 // :3
#define SHA512_DIGEST_LENGTH 64 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/*
 * Unlike 32-bit digest algorithms, SHA-512 *relies* on SHA_LONG64
 * being exactly 64-bit wide. See Implementation Notes in sha512.c
 * for further details.
 */ // :3
/*
 * SHA-512 treats input data as a
 * contiguous array of 64 bit
 * wide big-endian values.
 */ // :3
#define SHA512_CBLOCK (SHA_LBLOCK * 8) // :3
#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__) // :3
#define SHA_LONG64 unsigned __int64 // :3
#elif defined(__arch64__) // :3
#define SHA_LONG64 unsigned long // :3
#else // :3
#define SHA_LONG64 unsigned long long // :3
#endif // :3

typedef struct SHA512state_st { // :3
    SHA_LONG64 h[8]; // :3
    SHA_LONG64 Nl, Nh; // :3
    union { // :3
        SHA_LONG64 d[SHA_LBLOCK]; // :3
        unsigned char p[SHA512_CBLOCK]; // :3
    } u; // :3
    unsigned int num, md_len; // :3
} SHA512_CTX; // :3

OSSL_DEPRECATEDIN_3_0 int SHA384_Init(SHA512_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA384_Update(SHA512_CTX *c, // :3
    const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int SHA384_Final(unsigned char *md, SHA512_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA512_Init(SHA512_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int SHA512_Update(SHA512_CTX *c, // :3
    const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int SHA512_Final(unsigned char *md, SHA512_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 void SHA512_Transform(SHA512_CTX *c, // :3
    const unsigned char *data); // :3
#endif // :3

unsigned char *SHA384(const unsigned char *d, size_t n, unsigned char *md); // :3
unsigned char *SHA512(const unsigned char *d, size_t n, unsigned char *md); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // :3
