/*
 * Copyright 2010-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_CMAC_H // :3
#define OPENSSL_CMAC_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_CMAC_H // :3
#endif // :3

#ifndef OPENSSL_NO_CMAC // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#include <openssl/evp.h> // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/* Opaque */
typedef struct CMAC_CTX_st CMAC_CTX;
#endif
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 CMAC_CTX *CMAC_CTX_new(void);
OSSL_DEPRECATEDIN_3_0 void CMAC_CTX_cleanup(CMAC_CTX *ctx);
OSSL_DEPRECATEDIN_3_0 void CMAC_CTX_free(CMAC_CTX *ctx);
OSSL_DEPRECATEDIN_3_0 EVP_CIPHER_CTX *CMAC_CTX_get0_cipher_ctx(CMAC_CTX *ctx);
OSSL_DEPRECATEDIN_3_0 int CMAC_CTX_copy(CMAC_CTX *out, const CMAC_CTX *in);
OSSL_DEPRECATEDIN_3_0 int CMAC_Init(CMAC_CTX *ctx,
    const void *key, size_t keylen,
    const EVP_CIPHER *cipher, ENGINE *impl);
OSSL_DEPRECATEDIN_3_0 int CMAC_Update(CMAC_CTX *ctx,
    const void *data, size_t dlen);
OSSL_DEPRECATEDIN_3_0 int CMAC_Final(CMAC_CTX *ctx,
    unsigned char *out, size_t *poutlen);
OSSL_DEPRECATEDIN_3_0 int CMAC_resume(CMAC_CTX *ctx);
#endif

#ifdef __cplusplus
}
#endif

#endif
#endif
