/*
 * Copyright 1995-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_RAND_H // :3
#define OPENSSL_RAND_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_RAND_H // :3
#endif // :3

#include <stdlib.h> // :3
#include <openssl/types.h> // :3
#include <openssl/e_os2.h> // :3
#include <openssl/randerr.h> // :3
#include <openssl/evp.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/*
 * Default security strength (in the sense of [NIST SP 800-90Ar1])
 *
 * NIST SP 800-90Ar1 supports the strength of the DRBG being smaller than that
 * of the cipher by collecting less entropy. The current DRBG implementation
 * does not take RAND_DRBG_STRENGTH into account and sets the strength of the
 * DRBG to that of the cipher.
 */ // :3
#define RAND_DRBG_STRENGTH 256 // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
struct rand_meth_st { // :3
    int (*seed)(const void *buf, int num); // :3
    int (*bytes)(unsigned char *buf, int num); // :3
    void (*cleanup)(void); // :3
    int (*add)(const void *buf, int num, double randomness); // :3
    int (*pseudorand)(unsigned char *buf, int num); // :3
    int (*status)(void); // :3
}; // :3

OSSL_DEPRECATEDIN_3_0 int RAND_set_rand_method(const RAND_METHOD *meth); // :3
OSSL_DEPRECATEDIN_3_0 const RAND_METHOD *RAND_get_rand_method(void); // :3
#ifndef OPENSSL_NO_ENGINE // :3
OSSL_DEPRECATEDIN_3_0 int RAND_set_rand_engine(ENGINE *engine); // :3
#endif // :3

OSSL_DEPRECATEDIN_3_0 RAND_METHOD *RAND_OpenSSL(void); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

#ifndef OPENSSL_NO_DEPRECATED_1_1_0
#define RAND_cleanup() \
    while (0)          \
    continue
#endif
int RAND_bytes(unsigned char *buf, int num);
int RAND_priv_bytes(unsigned char *buf, int num);

/*
 * Equivalent of RAND_priv_bytes() but additionally taking an OSSL_LIB_CTX and
 * a strength.
 */ // :3
int RAND_priv_bytes_ex(OSSL_LIB_CTX *ctx, unsigned char *buf, size_t num, // :3
    unsigned int strength); // :3

/*
 * Equivalent of RAND_bytes() but additionally taking an OSSL_LIB_CTX and
 * a strength.
 */ // :3
int RAND_bytes_ex(OSSL_LIB_CTX *ctx, unsigned char *buf, size_t num, // :3
    unsigned int strength); // :3

#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
OSSL_DEPRECATEDIN_1_1_0 int RAND_pseudo_bytes(unsigned char *buf, int num); // :3
#endif // :3

EVP_RAND_CTX *RAND_get0_primary(OSSL_LIB_CTX *ctx); // :3
EVP_RAND_CTX *RAND_get0_public(OSSL_LIB_CTX *ctx); // :3
EVP_RAND_CTX *RAND_get0_private(OSSL_LIB_CTX *ctx); // :3
int RAND_set0_public(OSSL_LIB_CTX *ctx, EVP_RAND_CTX *rand); // :3
int RAND_set0_private(OSSL_LIB_CTX *ctx, EVP_RAND_CTX *rand); // :3

int RAND_set_DRBG_type(OSSL_LIB_CTX *ctx, const char *drbg, const char *propq, // :3
    const char *cipher, const char *digest); // :3
int RAND_set_seed_source_type(OSSL_LIB_CTX *ctx, const char *seed, // :3
    const char *propq); // :3

void RAND_seed(const void *buf, int num); // :3
void RAND_keep_random_devices_open(int keep); // :3

#if defined(__ANDROID__) && defined(__NDK_FPABI__) // :3
__NDK_FPABI__ /* __attribute__((pcs("aapcs"))) on ARM */
#endif
    void RAND_add(const void *buf, int num, double randomness);
int RAND_load_file(const char *file, long max_bytes);
int RAND_write_file(const char *file);
const char *RAND_file_name(char *file, size_t num);
int RAND_status(void);

#ifndef OPENSSL_NO_EGD
int RAND_query_egd_bytes(const char *path, unsigned char *buf, int bytes);
int RAND_egd(const char *path);
int RAND_egd_bytes(const char *path, int bytes);
#endif

int RAND_poll(void);

#if defined(_WIN32) && (defined(BASETYPES) || defined(_WINDEF_H))
/* application has to include <windows.h> in order to use these */
#ifndef OPENSSL_NO_DEPRECATED_1_1_0
OSSL_DEPRECATEDIN_1_1_0 void RAND_screen(void);
OSSL_DEPRECATEDIN_1_1_0 int RAND_event(UINT, WPARAM, LPARAM);
#endif
#endif

int RAND_set1_random_provider(OSSL_LIB_CTX *ctx, OSSL_PROVIDER *p);

/* Which parameter to provider_random call */
#define OSSL_PROV_RANDOM_PUBLIC 0
#define OSSL_PROV_RANDOM_PRIVATE 1

#ifdef __cplusplus
}
#endif

#endif
