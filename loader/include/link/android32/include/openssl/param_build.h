/*
 * Copyright 2019-2021 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2019, Oracle and/or its affiliates.  All rights reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_PARAM_BUILD_H // :3
#define OPENSSL_PARAM_BUILD_H // :3
#pragma once // :3

#include <openssl/params.h> // :3
#include <openssl/types.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

OSSL_PARAM_BLD *OSSL_PARAM_BLD_new(void); // :3
OSSL_PARAM *OSSL_PARAM_BLD_to_param(OSSL_PARAM_BLD *bld); // :3
void OSSL_PARAM_BLD_free(OSSL_PARAM_BLD *bld); // :3

int OSSL_PARAM_BLD_push_int(OSSL_PARAM_BLD *bld, const char *key, int val); // :3
int OSSL_PARAM_BLD_push_uint(OSSL_PARAM_BLD *bld, const char *key, // :3
    unsigned int val); // :3
int OSSL_PARAM_BLD_push_long(OSSL_PARAM_BLD *bld, const char *key, // :3
    long int val); // :3
int OSSL_PARAM_BLD_push_ulong(OSSL_PARAM_BLD *bld, const char *key, // :3
    unsigned long int val); // :3
int OSSL_PARAM_BLD_push_int32(OSSL_PARAM_BLD *bld, const char *key, // :3
    int32_t val); // :3
int OSSL_PARAM_BLD_push_uint32(OSSL_PARAM_BLD *bld, const char *key, // :3
    uint32_t val); // :3
int OSSL_PARAM_BLD_push_int64(OSSL_PARAM_BLD *bld, const char *key, // :3
    int64_t val); // :3
int OSSL_PARAM_BLD_push_uint64(OSSL_PARAM_BLD *bld, const char *key, // :3
    uint64_t val); // :3
int OSSL_PARAM_BLD_push_size_t(OSSL_PARAM_BLD *bld, const char *key, // :3
    size_t val); // :3
int OSSL_PARAM_BLD_push_time_t(OSSL_PARAM_BLD *bld, const char *key, // :3
    time_t val); // :3
int OSSL_PARAM_BLD_push_double(OSSL_PARAM_BLD *bld, const char *key, // :3
    double val); // :3
int OSSL_PARAM_BLD_push_BN(OSSL_PARAM_BLD *bld, const char *key, // :3
    const BIGNUM *bn); // :3
int OSSL_PARAM_BLD_push_BN_pad(OSSL_PARAM_BLD *bld, const char *key, // :3
    const BIGNUM *bn, size_t sz); // :3
int OSSL_PARAM_BLD_push_utf8_string(OSSL_PARAM_BLD *bld, const char *key, // :3
    const char *buf, size_t bsize); // :3
int OSSL_PARAM_BLD_push_utf8_ptr(OSSL_PARAM_BLD *bld, const char *key, // :3
    char *buf, size_t bsize); // :3
int OSSL_PARAM_BLD_push_octet_string(OSSL_PARAM_BLD *bld, const char *key, // :3
    const void *buf, size_t bsize); // :3
int OSSL_PARAM_BLD_push_octet_ptr(OSSL_PARAM_BLD *bld, const char *key, // :3
    void *buf, size_t bsize); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif /* OPENSSL_PARAM_BUILD_H */
