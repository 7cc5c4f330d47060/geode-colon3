/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_BLOWFISH_H // :3
#define OPENSSL_BLOWFISH_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_BLOWFISH_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_BF // :3
#include <openssl/e_os2.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define BF_BLOCK 8 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3

#define BF_ENCRYPT 1 // :3
#define BF_DECRYPT 0 // :3

/*-
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * ! BF_LONG has to be at least 32 bits wide.                     !
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */ // :3
#define BF_LONG unsigned int // :3

#define BF_ROUNDS 16 // :3

typedef struct bf_key_st { // :3
    BF_LONG P[BF_ROUNDS + 2]; // :3
    BF_LONG S[4 * 256]; // :3
} BF_KEY; // :3

#endif /* OPENSSL_NO_DEPRECATED_3_0 */
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 void BF_set_key(BF_KEY *key, int len,
    const unsigned char *data);
OSSL_DEPRECATEDIN_3_0 void BF_encrypt(BF_LONG *data, const BF_KEY *key);
OSSL_DEPRECATEDIN_3_0 void BF_decrypt(BF_LONG *data, const BF_KEY *key);
OSSL_DEPRECATEDIN_3_0 void BF_ecb_encrypt(const unsigned char *in,
    unsigned char *out, const BF_KEY *key,
    int enc);
OSSL_DEPRECATEDIN_3_0 void BF_cbc_encrypt(const unsigned char *in,
    unsigned char *out, long length,
    const BF_KEY *schedule,
    unsigned char *ivec, int enc);
OSSL_DEPRECATEDIN_3_0 void BF_cfb64_encrypt(const unsigned char *in,
    unsigned char *out,
    long length, const BF_KEY *schedule,
    unsigned char *ivec, int *num,
    int enc);
OSSL_DEPRECATEDIN_3_0 void BF_ofb64_encrypt(const unsigned char *in,
    unsigned char *out,
    long length, const BF_KEY *schedule,
    unsigned char *ivec, int *num);
OSSL_DEPRECATEDIN_3_0 const char *BF_options(void);
#endif

#ifdef __cplusplus
}
#endif
#endif

#endif
