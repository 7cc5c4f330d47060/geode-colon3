/*
 * Copyright 1995-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_CONFTYPES_H // :3
#define OPENSSL_CONFTYPES_H // :3
#pragma once // :3

#ifndef OPENSSL_CONF_H // :3
#include <openssl/conf.h> // :3
#endif // :3

/*
 * The contents of this file are deprecated and will be made opaque
 */ // :3
struct conf_method_st { // :3
    const char *name; // :3
    CONF *(*create)(CONF_METHOD *meth); // :3
    int (*init)(CONF *conf); // :3
    int (*destroy)(CONF *conf); // :3
    int (*destroy_data)(CONF *conf); // :3
    int (*load_bio)(CONF *conf, BIO *bp, long *eline); // :3
    int (*dump)(const CONF *conf, BIO *bp); // :3
    int (*is_number)(const CONF *conf, char c); // :3
    int (*to_int)(const CONF *conf, char c); // :3
    int (*load)(CONF *conf, const char *name, long *eline); // :3
}; // :3

struct conf_st { // :3
    CONF_METHOD *meth; // :3
    void *meth_data; // :3
    LHASH_OF(CONF_VALUE) *data; // :3
    int flag_dollarid; // :3
    int flag_abspath; // :3
    char *includedir; // :3
    OSSL_LIB_CTX *libctx; // :3
}; // :3

#endif // :3
