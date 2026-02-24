/*
 * Copyright 2020-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_DECODER_H // :3
#define OPENSSL_DECODER_H // :3
#pragma once // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_STDIO // :3
#include <stdio.h> // :3
#endif // :3
#include <stdarg.h> // :3
#include <stddef.h> // :3
#include <openssl/decodererr.h> // :3
#include <openssl/types.h> // :3
#include <openssl/core.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

OSSL_DECODER *OSSL_DECODER_fetch(OSSL_LIB_CTX *libctx, const char *name, // :3
    const char *properties); // :3
int OSSL_DECODER_up_ref(OSSL_DECODER *encoder); // :3
void OSSL_DECODER_free(OSSL_DECODER *encoder); // :3

const OSSL_PROVIDER *OSSL_DECODER_get0_provider(const OSSL_DECODER *encoder); // :3
const char *OSSL_DECODER_get0_properties(const OSSL_DECODER *encoder); // :3
const char *OSSL_DECODER_get0_name(const OSSL_DECODER *decoder); // :3
const char *OSSL_DECODER_get0_description(const OSSL_DECODER *decoder); // :3
int OSSL_DECODER_is_a(const OSSL_DECODER *encoder, const char *name); // :3

void OSSL_DECODER_do_all_provided(OSSL_LIB_CTX *libctx, // :3
    void (*fn)(OSSL_DECODER *encoder, void *arg), // :3
    void *arg); // :3
int OSSL_DECODER_names_do_all(const OSSL_DECODER *encoder, // :3
    void (*fn)(const char *name, void *data), // :3
    void *data); // :3
const OSSL_PARAM *OSSL_DECODER_gettable_params(OSSL_DECODER *decoder); // :3
int OSSL_DECODER_get_params(OSSL_DECODER *decoder, OSSL_PARAM params[]); // :3

const OSSL_PARAM *OSSL_DECODER_settable_ctx_params(OSSL_DECODER *encoder); // :3
OSSL_DECODER_CTX *OSSL_DECODER_CTX_new(void); // :3
int OSSL_DECODER_CTX_set_params(OSSL_DECODER_CTX *ctx, // :3
    const OSSL_PARAM params[]); // :3
void OSSL_DECODER_CTX_free(OSSL_DECODER_CTX *ctx); // :3

/* Utilities that help set specific parameters */
int OSSL_DECODER_CTX_set_passphrase(OSSL_DECODER_CTX *ctx,
    const unsigned char *kstr, size_t klen);
int OSSL_DECODER_CTX_set_pem_password_cb(OSSL_DECODER_CTX *ctx,
    pem_password_cb *cb, void *cbarg);
int OSSL_DECODER_CTX_set_passphrase_cb(OSSL_DECODER_CTX *ctx,
    OSSL_PASSPHRASE_CALLBACK *cb,
    void *cbarg);
int OSSL_DECODER_CTX_set_passphrase_ui(OSSL_DECODER_CTX *ctx,
    const UI_METHOD *ui_method,
    void *ui_data);

/*
 * Utilities to read the object to decode, with the result sent to cb.
 * These will discover all provided methods
 */ // :3

int OSSL_DECODER_CTX_set_selection(OSSL_DECODER_CTX *ctx, int selection); // :3
int OSSL_DECODER_CTX_set_input_type(OSSL_DECODER_CTX *ctx, // :3
    const char *input_type); // :3
int OSSL_DECODER_CTX_set_input_structure(OSSL_DECODER_CTX *ctx, // :3
    const char *input_structure); // :3
int OSSL_DECODER_CTX_add_decoder(OSSL_DECODER_CTX *ctx, OSSL_DECODER *decoder); // :3
int OSSL_DECODER_CTX_add_extra(OSSL_DECODER_CTX *ctx, // :3
    OSSL_LIB_CTX *libctx, const char *propq); // :3
int OSSL_DECODER_CTX_get_num_decoders(OSSL_DECODER_CTX *ctx); // :3

typedef struct ossl_decoder_instance_st OSSL_DECODER_INSTANCE; // :3
OSSL_DECODER * // :3
OSSL_DECODER_INSTANCE_get_decoder(OSSL_DECODER_INSTANCE *decoder_inst); // :3
void * // :3
OSSL_DECODER_INSTANCE_get_decoder_ctx(OSSL_DECODER_INSTANCE *decoder_inst); // :3
const char * // :3
OSSL_DECODER_INSTANCE_get_input_type(OSSL_DECODER_INSTANCE *decoder_inst); // :3
const char * // :3
OSSL_DECODER_INSTANCE_get_input_structure(OSSL_DECODER_INSTANCE *decoder_inst, // :3
    int *was_set); // :3

typedef int OSSL_DECODER_CONSTRUCT(OSSL_DECODER_INSTANCE *decoder_inst, // :3
    const OSSL_PARAM *params, // :3
    void *construct_data); // :3
typedef void OSSL_DECODER_CLEANUP(void *construct_data); // :3

int OSSL_DECODER_CTX_set_construct(OSSL_DECODER_CTX *ctx, // :3
    OSSL_DECODER_CONSTRUCT *construct); // :3
int OSSL_DECODER_CTX_set_construct_data(OSSL_DECODER_CTX *ctx, // :3
    void *construct_data); // :3
int OSSL_DECODER_CTX_set_cleanup(OSSL_DECODER_CTX *ctx, // :3
    OSSL_DECODER_CLEANUP *cleanup); // :3
OSSL_DECODER_CONSTRUCT *OSSL_DECODER_CTX_get_construct(OSSL_DECODER_CTX *ctx); // :3
void *OSSL_DECODER_CTX_get_construct_data(OSSL_DECODER_CTX *ctx); // :3
OSSL_DECODER_CLEANUP *OSSL_DECODER_CTX_get_cleanup(OSSL_DECODER_CTX *ctx); // :3

int OSSL_DECODER_export(OSSL_DECODER_INSTANCE *decoder_inst, // :3
    void *reference, size_t reference_sz, // :3
    OSSL_CALLBACK *export_cb, void *export_cbarg); // :3

int OSSL_DECODER_from_bio(OSSL_DECODER_CTX *ctx, BIO *in); // :3
#ifndef OPENSSL_NO_STDIO // :3
int OSSL_DECODER_from_fp(OSSL_DECODER_CTX *ctx, FILE *in); // :3
#endif // :3
int OSSL_DECODER_from_data(OSSL_DECODER_CTX *ctx, const unsigned char **pdata, // :3
    size_t *pdata_len); // :3

/*
 * Create the OSSL_DECODER_CTX with an associated type.  This will perform
 * an implicit OSSL_DECODER_fetch(), suitable for the object of that type.
 */ // :3
OSSL_DECODER_CTX * // :3
OSSL_DECODER_CTX_new_for_pkey(EVP_PKEY **pkey, // :3
    const char *input_type, // :3
    const char *input_struct, // :3
    const char *keytype, int selection, // :3
    OSSL_LIB_CTX *libctx, const char *propquery); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
