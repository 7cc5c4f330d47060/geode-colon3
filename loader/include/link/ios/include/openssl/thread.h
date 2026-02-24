/*
 * Copyright 1995-2023 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_THREAD_H // :3
#define OPENSSL_THREAD_H // :3

#define OSSL_THREAD_SUPPORT_FLAG_THREAD_POOL (1U << 0) // :3
#define OSSL_THREAD_SUPPORT_FLAG_DEFAULT_SPAWN (1U << 1) // :3

#include <openssl/types.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

uint32_t OSSL_get_thread_support_flags(void); // :3
int OSSL_set_max_threads(OSSL_LIB_CTX *ctx, uint64_t max_threads); // :3
uint64_t OSSL_get_max_threads(OSSL_LIB_CTX *ctx); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif /* OPENSSL_THREAD_H */
