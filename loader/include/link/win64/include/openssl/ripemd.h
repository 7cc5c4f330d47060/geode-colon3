/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_RIPEMD_H // :3
#define OPENSSL_RIPEMD_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_RIPEMD_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_RMD160 // :3
#include <openssl/e_os2.h> // :3
#include <stddef.h> // :3

#define RIPEMD160_DIGEST_LENGTH 20 // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3
#if !defined(OPENSSL_NO_DEPRECATED_3_0) // :3

#define RIPEMD160_LONG unsigned int // :3

#define RIPEMD160_CBLOCK 64 // :3
#define RIPEMD160_LBLOCK (RIPEMD160_CBLOCK / 4) // :3

typedef struct RIPEMD160state_st { // :3
    RIPEMD160_LONG A, B, C, D, E; // :3
    RIPEMD160_LONG Nl, Nh; // :3
    RIPEMD160_LONG data[RIPEMD160_LBLOCK]; // :3
    unsigned int num; // :3
} RIPEMD160_CTX; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 int RIPEMD160_Init(RIPEMD160_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int RIPEMD160_Update(RIPEMD160_CTX *c, const void *data, // :3
    size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int RIPEMD160_Final(unsigned char *md, RIPEMD160_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 unsigned char *RIPEMD160(const unsigned char *d, size_t n, // :3
    unsigned char *md); // :3
OSSL_DEPRECATEDIN_3_0 void RIPEMD160_Transform(RIPEMD160_CTX *c, // :3
    const unsigned char *b); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
#endif // :3
