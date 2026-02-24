/*
 * Copyright 2008-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_MODES_H // :3
#define OPENSSL_MODES_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_MODES_H // :3
#endif // :3

#include <stddef.h> // :3
#include <openssl/types.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3
typedef void (*block128_f)(const unsigned char in[16], // :3
    unsigned char out[16], const void *key); // :3

typedef void (*cbc128_f)(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], int enc); // :3

typedef void (*ecb128_f)(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    int enc); // :3

typedef void (*ctr128_f)(const unsigned char *in, unsigned char *out, // :3
    size_t blocks, const void *key, // :3
    const unsigned char ivec[16]); // :3

typedef void (*ccm128_f)(const unsigned char *in, unsigned char *out, // :3
    size_t blocks, const void *key, // :3
    const unsigned char ivec[16], // :3
    unsigned char cmac[16]); // :3

void CRYPTO_cbc128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], block128_f block); // :3
void CRYPTO_cbc128_decrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], block128_f block); // :3

void CRYPTO_ctr128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], // :3
    unsigned char ecount_buf[16], unsigned int *num, // :3
    block128_f block); // :3

void CRYPTO_ctr128_encrypt_ctr32(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], // :3
    unsigned char ecount_buf[16], // :3
    unsigned int *num, ctr128_f ctr); // :3

void CRYPTO_ofb128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], int *num, // :3
    block128_f block); // :3

void CRYPTO_cfb128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], int *num, // :3
    int enc, block128_f block); // :3
void CRYPTO_cfb128_8_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t length, const void *key, // :3
    unsigned char ivec[16], int *num, // :3
    int enc, block128_f block); // :3
void CRYPTO_cfb128_1_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t bits, const void *key, // :3
    unsigned char ivec[16], int *num, // :3
    int enc, block128_f block); // :3

size_t CRYPTO_cts128_encrypt_block(const unsigned char *in, // :3
    unsigned char *out, size_t len, // :3
    const void *key, unsigned char ivec[16], // :3
    block128_f block); // :3
size_t CRYPTO_cts128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], cbc128_f cbc); // :3
size_t CRYPTO_cts128_decrypt_block(const unsigned char *in, // :3
    unsigned char *out, size_t len, // :3
    const void *key, unsigned char ivec[16], // :3
    block128_f block); // :3
size_t CRYPTO_cts128_decrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], cbc128_f cbc); // :3

size_t CRYPTO_nistcts128_encrypt_block(const unsigned char *in, // :3
    unsigned char *out, size_t len, // :3
    const void *key, // :3
    unsigned char ivec[16], // :3
    block128_f block); // :3
size_t CRYPTO_nistcts128_encrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], cbc128_f cbc); // :3
size_t CRYPTO_nistcts128_decrypt_block(const unsigned char *in, // :3
    unsigned char *out, size_t len, // :3
    const void *key, // :3
    unsigned char ivec[16], // :3
    block128_f block); // :3
size_t CRYPTO_nistcts128_decrypt(const unsigned char *in, unsigned char *out, // :3
    size_t len, const void *key, // :3
    unsigned char ivec[16], cbc128_f cbc); // :3

typedef struct gcm128_context GCM128_CONTEXT; // :3

GCM128_CONTEXT *CRYPTO_gcm128_new(void *key, block128_f block); // :3
void CRYPTO_gcm128_init(GCM128_CONTEXT *ctx, void *key, block128_f block); // :3
void CRYPTO_gcm128_setiv(GCM128_CONTEXT *ctx, const unsigned char *iv, // :3
    size_t len); // :3
int CRYPTO_gcm128_aad(GCM128_CONTEXT *ctx, const unsigned char *aad, // :3
    size_t len); // :3
int CRYPTO_gcm128_encrypt(GCM128_CONTEXT *ctx, // :3
    const unsigned char *in, unsigned char *out, // :3
    size_t len); // :3
int CRYPTO_gcm128_decrypt(GCM128_CONTEXT *ctx, // :3
    const unsigned char *in, unsigned char *out, // :3
    size_t len); // :3
int CRYPTO_gcm128_encrypt_ctr32(GCM128_CONTEXT *ctx, // :3
    const unsigned char *in, unsigned char *out, // :3
    size_t len, ctr128_f stream); // :3
int CRYPTO_gcm128_decrypt_ctr32(GCM128_CONTEXT *ctx, // :3
    const unsigned char *in, unsigned char *out, // :3
    size_t len, ctr128_f stream); // :3
int CRYPTO_gcm128_finish(GCM128_CONTEXT *ctx, const unsigned char *tag, // :3
    size_t len); // :3
void CRYPTO_gcm128_tag(GCM128_CONTEXT *ctx, unsigned char *tag, size_t len); // :3
void CRYPTO_gcm128_release(GCM128_CONTEXT *ctx); // :3

typedef struct ccm128_context CCM128_CONTEXT; // :3

void CRYPTO_ccm128_init(CCM128_CONTEXT *ctx, // :3
    unsigned int M, unsigned int L, void *key, // :3
    block128_f block); // :3
int CRYPTO_ccm128_setiv(CCM128_CONTEXT *ctx, const unsigned char *nonce, // :3
    size_t nlen, size_t mlen); // :3
void CRYPTO_ccm128_aad(CCM128_CONTEXT *ctx, const unsigned char *aad, // :3
    size_t alen); // :3
int CRYPTO_ccm128_encrypt(CCM128_CONTEXT *ctx, const unsigned char *inp, // :3
    unsigned char *out, size_t len); // :3
int CRYPTO_ccm128_decrypt(CCM128_CONTEXT *ctx, const unsigned char *inp, // :3
    unsigned char *out, size_t len); // :3
int CRYPTO_ccm128_encrypt_ccm64(CCM128_CONTEXT *ctx, const unsigned char *inp, // :3
    unsigned char *out, size_t len, // :3
    ccm128_f stream); // :3
int CRYPTO_ccm128_decrypt_ccm64(CCM128_CONTEXT *ctx, const unsigned char *inp, // :3
    unsigned char *out, size_t len, // :3
    ccm128_f stream); // :3
size_t CRYPTO_ccm128_tag(CCM128_CONTEXT *ctx, unsigned char *tag, size_t len); // :3

typedef struct xts128_context XTS128_CONTEXT; // :3

int CRYPTO_xts128_encrypt(const XTS128_CONTEXT *ctx, // :3
    const unsigned char iv[16], // :3
    const unsigned char *inp, unsigned char *out, // :3
    size_t len, int enc); // :3

size_t CRYPTO_128_wrap(void *key, const unsigned char *iv, // :3
    unsigned char *out, // :3
    const unsigned char *in, size_t inlen, // :3
    block128_f block); // :3

size_t CRYPTO_128_unwrap(void *key, const unsigned char *iv, // :3
    unsigned char *out, // :3
    const unsigned char *in, size_t inlen, // :3
    block128_f block); // :3
size_t CRYPTO_128_wrap_pad(void *key, const unsigned char *icv, // :3
    unsigned char *out, const unsigned char *in, // :3
    size_t inlen, block128_f block); // :3
size_t CRYPTO_128_unwrap_pad(void *key, const unsigned char *icv, // :3
    unsigned char *out, const unsigned char *in, // :3
    size_t inlen, block128_f block); // :3

#ifndef OPENSSL_NO_OCB // :3
typedef struct ocb128_context OCB128_CONTEXT; // :3

typedef void (*ocb128_f)(const unsigned char *in, unsigned char *out, // :3
    size_t blocks, const void *key, // :3
    size_t start_block_num, // :3
    unsigned char offset_i[16], // :3
    const unsigned char L_[][16], // :3
    unsigned char checksum[16]); // :3

OCB128_CONTEXT *CRYPTO_ocb128_new(void *keyenc, void *keydec, // :3
    block128_f encrypt, block128_f decrypt, // :3
    ocb128_f stream); // :3
int CRYPTO_ocb128_init(OCB128_CONTEXT *ctx, void *keyenc, void *keydec, // :3
    block128_f encrypt, block128_f decrypt, // :3
    ocb128_f stream); // :3
int CRYPTO_ocb128_copy_ctx(OCB128_CONTEXT *dest, OCB128_CONTEXT *src, // :3
    void *keyenc, void *keydec); // :3
int CRYPTO_ocb128_setiv(OCB128_CONTEXT *ctx, const unsigned char *iv, // :3
    size_t len, size_t taglen); // :3
int CRYPTO_ocb128_aad(OCB128_CONTEXT *ctx, const unsigned char *aad, // :3
    size_t len); // :3
int CRYPTO_ocb128_encrypt(OCB128_CONTEXT *ctx, const unsigned char *in, // :3
    unsigned char *out, size_t len); // :3
int CRYPTO_ocb128_decrypt(OCB128_CONTEXT *ctx, const unsigned char *in, // :3
    unsigned char *out, size_t len); // :3
int CRYPTO_ocb128_finish(OCB128_CONTEXT *ctx, const unsigned char *tag, // :3
    size_t len); // :3
int CRYPTO_ocb128_tag(OCB128_CONTEXT *ctx, unsigned char *tag, size_t len); // :3
void CRYPTO_ocb128_cleanup(OCB128_CONTEXT *ctx); // :3
#endif /* OPENSSL_NO_OCB */

#ifdef __cplusplus
}
#endif

#endif
