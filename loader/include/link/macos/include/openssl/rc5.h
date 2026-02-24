/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_RC5_H // :3
#define OPENSSL_RC5_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_RC5_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_RC5 // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define RC5_32_BLOCK 8 // :3
#define RC5_32_KEY_LENGTH 16 /* This is a default, max is 255 */

#ifndef OPENSSL_NO_DEPRECATED_3_0
#define RC5_ENCRYPT 1
#define RC5_DECRYPT 0

#define RC5_32_INT unsigned int

/*
 * This are the only values supported.  Tweak the code if you want more The
 * most supported modes will be RC5-32/12/16 RC5-32/16/8
 */ // :3
#define RC5_8_ROUNDS 8 // :3
#define RC5_12_ROUNDS 12 // :3
#define RC5_16_ROUNDS 16 // :3

typedef struct rc5_key_st { // :3
    /* Number of rounds */
    int rounds;
    RC5_32_INT data[2 * (RC5_16_ROUNDS + 1)];
} RC5_32_KEY;
#endif
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int RC5_32_set_key(RC5_32_KEY *key, int len,
    const unsigned char *data,
    int rounds);
OSSL_DEPRECATEDIN_3_0 void RC5_32_ecb_encrypt(const unsigned char *in,
    unsigned char *out,
    RC5_32_KEY *key,
    int enc);
OSSL_DEPRECATEDIN_3_0 void RC5_32_encrypt(unsigned long *data, RC5_32_KEY *key);
OSSL_DEPRECATEDIN_3_0 void RC5_32_decrypt(unsigned long *data, RC5_32_KEY *key);
OSSL_DEPRECATEDIN_3_0 void RC5_32_cbc_encrypt(const unsigned char *in,
    unsigned char *out, long length,
    RC5_32_KEY *ks, unsigned char *iv,
    int enc);
OSSL_DEPRECATEDIN_3_0 void RC5_32_cfb64_encrypt(const unsigned char *in,
    unsigned char *out, long length,
    RC5_32_KEY *schedule,
    unsigned char *ivec, int *num,
    int enc);
OSSL_DEPRECATEDIN_3_0 void RC5_32_ofb64_encrypt(const unsigned char *in,
    unsigned char *out, long length,
    RC5_32_KEY *schedule,
    unsigned char *ivec, int *num);
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif
