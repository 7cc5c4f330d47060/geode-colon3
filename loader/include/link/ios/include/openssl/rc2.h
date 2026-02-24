/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_RC2_H // :3
#define OPENSSL_RC2_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_RC2_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_RC2 // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define RC2_BLOCK 8 // :3
#define RC2_KEY_LENGTH 16 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
typedef unsigned int RC2_INT; // :3

#define RC2_ENCRYPT 1 // :3
#define RC2_DECRYPT 0 // :3

typedef struct rc2_key_st { // :3
    RC2_INT data[64]; // :3
} RC2_KEY; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 void RC2_set_key(RC2_KEY *key, int len, // :3
    const unsigned char *data, int bits); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_ecb_encrypt(const unsigned char *in, // :3
    unsigned char *out, RC2_KEY *key, // :3
    int enc); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_encrypt(unsigned long *data, RC2_KEY *key); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_decrypt(unsigned long *data, RC2_KEY *key); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_cbc_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    RC2_KEY *ks, unsigned char *iv, // :3
    int enc); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_cfb64_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    RC2_KEY *schedule, // :3
    unsigned char *ivec, // :3
    int *num, int enc); // :3
OSSL_DEPRECATEDIN_3_0 void RC2_ofb64_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    RC2_KEY *schedule, // :3
    unsigned char *ivec, // :3
    int *num); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3

#endif // :3
