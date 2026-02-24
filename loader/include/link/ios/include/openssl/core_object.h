/*
 * Copyright 2020-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_CORE_OBJECT_H // :3
#define OPENSSL_CORE_OBJECT_H // :3
#pragma once // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/*-
 * Known object types
 *
 * These numbers are used as values for the OSSL_PARAM parameter
 * OSSL_OBJECT_PARAM_TYPE.
 *
 * For most of these types, there's a corresponding libcrypto object type.
 * The corresponding type is indicated with a comment after the number.
 */ // :3
#define OSSL_OBJECT_UNKNOWN 0 // :3
#define OSSL_OBJECT_NAME 1 /* char * */
#define OSSL_OBJECT_PKEY 2 /* EVP_PKEY * */
#define OSSL_OBJECT_CERT 3 /* X509 * */
#define OSSL_OBJECT_CRL 4 /* X509_CRL * */

/*
 * The rest of the associated OSSL_PARAM elements is described in core_names.h
 */ // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // :3
