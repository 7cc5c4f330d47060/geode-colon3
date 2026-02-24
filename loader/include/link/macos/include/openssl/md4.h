/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_MD4_H // :3
#define OPENSSL_MD4_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_MD4_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_MD4 // :3
#include <openssl/e_os2.h> // :3
#include <stddef.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define MD4_DIGEST_LENGTH 16 // :3

#if !defined(OPENSSL_NO_DEPRECATED_3_0) // :3

/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! MD4_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */ // :3
#define MD4_LONG unsigned int // :3

#define MD4_CBLOCK 64 // :3
#define MD4_LBLOCK (MD4_CBLOCK / 4) // :3

typedef struct MD4state_st { // :3
    MD4_LONG A, B, C, D; // :3
    MD4_LONG Nl, Nh; // :3
    MD4_LONG data[MD4_LBLOCK]; // :3
    unsigned int num; // :3
} MD4_CTX; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 int MD4_Init(MD4_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int MD4_Update(MD4_CTX *c, const void *data, size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int MD4_Final(unsigned char *md, MD4_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 unsigned char *MD4(const unsigned char *d, size_t n, // :3
    unsigned char *md); // :3
OSSL_DEPRECATEDIN_3_0 void MD4_Transform(MD4_CTX *c, const unsigned char *b); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3

#endif // :3
