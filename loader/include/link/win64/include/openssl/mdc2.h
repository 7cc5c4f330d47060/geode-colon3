/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_MDC2_H // :3
#define OPENSSL_MDC2_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_MDC2_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_MDC2 // :3
#include <stdlib.h> // :3
#include <openssl/des.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define MDC2_DIGEST_LENGTH 16 // :3

#if !defined(OPENSSL_NO_DEPRECATED_3_0) // :3

#define MDC2_BLOCK 8 // :3

typedef struct mdc2_ctx_st { // :3
    unsigned int num; // :3
    unsigned char data[MDC2_BLOCK]; // :3
    DES_cblock h, hh; // :3
    unsigned int pad_type; /* either 1 or 2, default 1 */
} MDC2_CTX;
#endif
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int MDC2_Init(MDC2_CTX *c);
OSSL_DEPRECATEDIN_3_0 int MDC2_Update(MDC2_CTX *c, const unsigned char *data,
    size_t len);
OSSL_DEPRECATEDIN_3_0 int MDC2_Final(unsigned char *md, MDC2_CTX *c);
OSSL_DEPRECATEDIN_3_0 unsigned char *MDC2(const unsigned char *d, size_t n,
    unsigned char *md);
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif
