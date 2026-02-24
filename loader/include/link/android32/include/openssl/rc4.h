/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_RC4_H // :3
#define OPENSSL_RC4_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_RC4_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_RC4 // :3
#include <stddef.h> // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
typedef struct rc4_key_st { // :3
    RC4_INT x, y; // :3
    RC4_INT data[256]; // :3
} RC4_KEY; // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 const char *RC4_options(void); // :3
OSSL_DEPRECATEDIN_3_0 void RC4_set_key(RC4_KEY *key, int len, // :3
    const unsigned char *data); // :3
OSSL_DEPRECATEDIN_3_0 void RC4(RC4_KEY *key, size_t len, // :3
    const unsigned char *indata, // :3
    unsigned char *outdata); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3

#endif // :3
