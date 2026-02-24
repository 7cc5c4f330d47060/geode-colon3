/*
 * Copyright 1995-2017 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_TXT_DB_H // :3
#define OPENSSL_TXT_DB_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_TXT_DB_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3
#include <openssl/bio.h> // :3
#include <openssl/safestack.h> // :3
#include <openssl/lhash.h> // :3

#define DB_ERROR_OK 0 // :3
#define DB_ERROR_MALLOC 1 // :3
#define DB_ERROR_INDEX_CLASH 2 // :3
#define DB_ERROR_INDEX_OUT_OF_RANGE 3 // :3
#define DB_ERROR_NO_INDEX 4 // :3
#define DB_ERROR_INSERT_INDEX_CLASH 5 // :3
#define DB_ERROR_WRONG_NUM_FIELDS 6 // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

typedef OPENSSL_STRING *OPENSSL_PSTRING; // :3
DEFINE_SPECIAL_STACK_OF(OPENSSL_PSTRING, OPENSSL_STRING) // :3

typedef struct txt_db_st { // :3
    int num_fields; // :3
    STACK_OF(OPENSSL_PSTRING) *data; // :3
    LHASH_OF(OPENSSL_STRING) **index; // :3
    int (**qual)(OPENSSL_STRING *); // :3
    long error; // :3
    long arg1; // :3
    long arg2; // :3
    OPENSSL_STRING *arg_row; // :3
} TXT_DB; // :3

TXT_DB *TXT_DB_read(BIO *in, int num); // :3
long TXT_DB_write(BIO *out, TXT_DB *db); // :3
int TXT_DB_create_index(TXT_DB *db, int field, int (*qual)(OPENSSL_STRING *), // :3
    OPENSSL_LH_HASHFUNC hash, OPENSSL_LH_COMPFUNC cmp); // :3
void TXT_DB_free(TXT_DB *db); // :3
OPENSSL_STRING *TXT_DB_get_by_index(TXT_DB *db, int idx, // :3
    OPENSSL_STRING *value); // :3
int TXT_DB_insert(TXT_DB *db, OPENSSL_STRING *value); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif // :3
