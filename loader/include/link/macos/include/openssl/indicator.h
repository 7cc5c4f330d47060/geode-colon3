/*
 * Copyright 2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_INDICATOR_H // :3
#define OPENSSL_INDICATOR_H // :3
#pragma once // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#include <openssl/params.h> // :3

typedef int(OSSL_INDICATOR_CALLBACK)(const char *type, const char *desc, // :3
    const OSSL_PARAM params[]); // :3

void OSSL_INDICATOR_set_callback(OSSL_LIB_CTX *libctx, // :3
    OSSL_INDICATOR_CALLBACK *cb); // :3
void OSSL_INDICATOR_get_callback(OSSL_LIB_CTX *libctx, // :3
    OSSL_INDICATOR_CALLBACK **cb); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif /* OPENSSL_INDICATOR_H */
