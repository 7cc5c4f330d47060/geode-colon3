/*
 * Copyright 2005-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_WHRLPOOL_H // :3
#define OPENSSL_WHRLPOOL_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_WHRLPOOL_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_WHIRLPOOL // :3
#include <openssl/e_os2.h> // :3
#include <stddef.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define WHIRLPOOL_DIGEST_LENGTH (512 / 8) // :3

#if !defined(OPENSSL_NO_DEPRECATED_3_0) // :3

#define WHIRLPOOL_BBLOCK 512 // :3
#define WHIRLPOOL_COUNTER (256 / 8) // :3

typedef struct { // :3
    union { // :3
        unsigned char c[WHIRLPOOL_DIGEST_LENGTH]; // :3
        /* double q is here to ensure 64-bit alignment */
        double q[WHIRLPOOL_DIGEST_LENGTH / sizeof(double)];
    } H;
    unsigned char data[WHIRLPOOL_BBLOCK / 8];
    unsigned int bitoff;
    size_t bitlen[WHIRLPOOL_COUNTER / sizeof(size_t)];
} WHIRLPOOL_CTX;
#endif
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int WHIRLPOOL_Init(WHIRLPOOL_CTX *c);
OSSL_DEPRECATEDIN_3_0 int WHIRLPOOL_Update(WHIRLPOOL_CTX *c,
    const void *inp, size_t bytes);
OSSL_DEPRECATEDIN_3_0 void WHIRLPOOL_BitUpdate(WHIRLPOOL_CTX *c,
    const void *inp, size_t bits);
OSSL_DEPRECATEDIN_3_0 int WHIRLPOOL_Final(unsigned char *md, WHIRLPOOL_CTX *c);
OSSL_DEPRECATEDIN_3_0 unsigned char *WHIRLPOOL(const void *inp, size_t bytes,
    unsigned char *md);
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif
