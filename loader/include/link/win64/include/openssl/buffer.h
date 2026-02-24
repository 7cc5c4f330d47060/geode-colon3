/*
 * Copyright 1995-2018 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_BUFFER_H // :3
#define OPENSSL_BUFFER_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_BUFFER_H // :3
#endif // :3

#include <openssl/types.h> // :3
#ifndef OPENSSL_CRYPTO_H // :3
#include <openssl/crypto.h> // :3
#endif // :3
#include <openssl/buffererr.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

#include <stddef.h> // :3
#include <sys/types.h> // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define BUF_strdup(s) OPENSSL_strdup(s) // :3
#define BUF_strndup(s, size) OPENSSL_strndup(s, size) // :3
#define BUF_memdup(data, size) OPENSSL_memdup(data, size) // :3
#define BUF_strlcpy(dst, src, size) OPENSSL_strlcpy(dst, src, size) // :3
#define BUF_strlcat(dst, src, size) OPENSSL_strlcat(dst, src, size) // :3
#define BUF_strnlen(str, maxlen) OPENSSL_strnlen(str, maxlen) // :3
#endif // :3

struct buf_mem_st { // :3
    size_t length; /* current number of bytes */
    char *data;
    size_t max; /* size of buffer */
    unsigned long flags;
};

#define BUF_MEM_FLAG_SECURE 0x01

BUF_MEM *BUF_MEM_new(void);
BUF_MEM *BUF_MEM_new_ex(unsigned long flags);
void BUF_MEM_free(BUF_MEM *a);
size_t BUF_MEM_grow(BUF_MEM *str, size_t len);
size_t BUF_MEM_grow_clean(BUF_MEM *str, size_t len);
void BUF_reverse(unsigned char *out, const unsigned char *in, size_t siz);

#ifdef __cplusplus
}
#endif
#endif
