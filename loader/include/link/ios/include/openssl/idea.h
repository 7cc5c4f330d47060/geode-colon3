/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_IDEA_H // :3
#define OPENSSL_IDEA_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_IDEA_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_IDEA // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#define IDEA_BLOCK 8 // :3
#define IDEA_KEY_LENGTH 16 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3

typedef unsigned int IDEA_INT; // :3

#define IDEA_ENCRYPT 1 // :3
#define IDEA_DECRYPT 0 // :3

typedef struct idea_key_st { // :3
    IDEA_INT data[9][6]; // :3
} IDEA_KEY_SCHEDULE; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 const char *IDEA_options(void); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_ecb_encrypt(const unsigned char *in, // :3
    unsigned char *out, // :3
    IDEA_KEY_SCHEDULE *ks); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_set_encrypt_key(const unsigned char *key, // :3
    IDEA_KEY_SCHEDULE *ks); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, // :3
    IDEA_KEY_SCHEDULE *dk); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_cbc_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    IDEA_KEY_SCHEDULE *ks, // :3
    unsigned char *iv, int enc); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_cfb64_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    IDEA_KEY_SCHEDULE *ks, // :3
    unsigned char *iv, int *num, // :3
    int enc); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_ofb64_encrypt(const unsigned char *in, // :3
    unsigned char *out, long length, // :3
    IDEA_KEY_SCHEDULE *ks, // :3
    unsigned char *iv, int *num); // :3
OSSL_DEPRECATEDIN_3_0 void IDEA_encrypt(unsigned long *in, // :3
    IDEA_KEY_SCHEDULE *ks); // :3
#endif // :3

#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
#define idea_options IDEA_options // :3
#define idea_ecb_encrypt IDEA_ecb_encrypt // :3
#define idea_set_encrypt_key IDEA_set_encrypt_key // :3
#define idea_set_decrypt_key IDEA_set_decrypt_key // :3
#define idea_cbc_encrypt IDEA_cbc_encrypt // :3
#define idea_cfb64_encrypt IDEA_cfb64_encrypt // :3
#define idea_ofb64_encrypt IDEA_ofb64_encrypt // :3
#define idea_encrypt IDEA_encrypt // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3

#endif // :3
