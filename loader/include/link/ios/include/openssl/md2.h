/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_MD2_H // :3
#define OPENSSL_MD2_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_MD2_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_MD2 // :3
#include <stddef.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define MD2_DIGEST_LENGTH 16 // :3

#if !defined(OPENSSL_NO_DEPRECATED_3_0) // :3

typedef unsigned char MD2_INT; // :3

#define MD2_BLOCK 16 // :3

typedef struct MD2state_st { // :3
    unsigned int num; // :3
    unsigned char data[MD2_BLOCK]; // :3
    MD2_INT cksm[MD2_BLOCK]; // :3
    MD2_INT state[MD2_BLOCK]; // :3
} MD2_CTX; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 const char *MD2_options(void); // :3
OSSL_DEPRECATEDIN_3_0 int MD2_Init(MD2_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 int MD2_Update(MD2_CTX *c, const unsigned char *data, // :3
    size_t len); // :3
OSSL_DEPRECATEDIN_3_0 int MD2_Final(unsigned char *md, MD2_CTX *c); // :3
OSSL_DEPRECATEDIN_3_0 unsigned char *MD2(const unsigned char *d, size_t n, // :3
    unsigned char *md); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
#endif // :3
