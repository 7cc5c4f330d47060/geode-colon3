/*
 * Copyright 2019-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_ENCODER_H // :3
#define OPENSSL_ENCODER_H // :3
#pragma once // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_STDIO // :3
#include <stdio.h> // :3
#endif // :3
#include <stdarg.h> // :3
#include <stddef.h> // :3
#include <openssl/encodererr.h> // :3
#include <openssl/types.h> // :3
#include <openssl/core.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

OSSL_ENCODER *OSSL_ENCODER_fetch(OSSL_LIB_CTX *libctx, const char *name, // :3
    const char *properties); // :3
int OSSL_ENCODER_up_ref(OSSL_ENCODER *encoder); // :3
void OSSL_ENCODER_free(OSSL_ENCODER *encoder); // :3

const OSSL_PROVIDER *OSSL_ENCODER_get0_provider(const OSSL_ENCODER *encoder); // :3
const char *OSSL_ENCODER_get0_properties(const OSSL_ENCODER *encoder); // :3
const char *OSSL_ENCODER_get0_name(const OSSL_ENCODER *kdf); // :3
const char *OSSL_ENCODER_get0_description(const OSSL_ENCODER *kdf); // :3
int OSSL_ENCODER_is_a(const OSSL_ENCODER *encoder, const char *name); // :3

void OSSL_ENCODER_do_all_provided(OSSL_LIB_CTX *libctx, // :3
    void (*fn)(OSSL_ENCODER *encoder, void *arg), // :3
    void *arg); // :3
int OSSL_ENCODER_names_do_all(const OSSL_ENCODER *encoder, // :3
    void (*fn)(const char *name, void *data), // :3
    void *data); // :3
const OSSL_PARAM *OSSL_ENCODER_gettable_params(OSSL_ENCODER *encoder); // :3
int OSSL_ENCODER_get_params(OSSL_ENCODER *encoder, OSSL_PARAM params[]); // :3

const OSSL_PARAM *OSSL_ENCODER_settable_ctx_params(OSSL_ENCODER *encoder); // :3
OSSL_ENCODER_CTX *OSSL_ENCODER_CTX_new(void); // :3
int OSSL_ENCODER_CTX_set_params(OSSL_ENCODER_CTX *ctx, // :3
    const OSSL_PARAM params[]); // :3
void OSSL_ENCODER_CTX_free(OSSL_ENCODER_CTX *ctx); // :3

/* Utilities that help set specific parameters */
int OSSL_ENCODER_CTX_set_passphrase(OSSL_ENCODER_CTX *ctx,
    const unsigned char *kstr, size_t klen);
int OSSL_ENCODER_CTX_set_pem_password_cb(OSSL_ENCODER_CTX *ctx,
    pem_password_cb *cb, void *cbarg);
int OSSL_ENCODER_CTX_set_passphrase_cb(OSSL_ENCODER_CTX *ctx,
    OSSL_PASSPHRASE_CALLBACK *cb,
    void *cbarg);
int OSSL_ENCODER_CTX_set_passphrase_ui(OSSL_ENCODER_CTX *ctx,
    const UI_METHOD *ui_method,
    void *ui_data);
int OSSL_ENCODER_CTX_set_cipher(OSSL_ENCODER_CTX *ctx,
    const char *cipher_name,
    const char *propquery);
int OSSL_ENCODER_CTX_set_selection(OSSL_ENCODER_CTX *ctx, int selection);
int OSSL_ENCODER_CTX_set_output_type(OSSL_ENCODER_CTX *ctx,
    const char *output_type);
int OSSL_ENCODER_CTX_set_output_structure(OSSL_ENCODER_CTX *ctx,
    const char *output_structure);

/* Utilities to add encoders */
int OSSL_ENCODER_CTX_add_encoder(OSSL_ENCODER_CTX *ctx, OSSL_ENCODER *encoder);
int OSSL_ENCODER_CTX_add_extra(OSSL_ENCODER_CTX *ctx,
    OSSL_LIB_CTX *libctx, const char *propq);
int OSSL_ENCODER_CTX_get_num_encoders(OSSL_ENCODER_CTX *ctx);

typedef struct ossl_encoder_instance_st OSSL_ENCODER_INSTANCE;
OSSL_ENCODER *
OSSL_ENCODER_INSTANCE_get_encoder(OSSL_ENCODER_INSTANCE *encoder_inst);
void *
OSSL_ENCODER_INSTANCE_get_encoder_ctx(OSSL_ENCODER_INSTANCE *encoder_inst);
const char *
OSSL_ENCODER_INSTANCE_get_output_type(OSSL_ENCODER_INSTANCE *encoder_inst);
const char *
OSSL_ENCODER_INSTANCE_get_output_structure(OSSL_ENCODER_INSTANCE *encoder_inst);

typedef const void *OSSL_ENCODER_CONSTRUCT(OSSL_ENCODER_INSTANCE *encoder_inst,
    void *construct_data);
typedef void OSSL_ENCODER_CLEANUP(void *construct_data);

int OSSL_ENCODER_CTX_set_construct(OSSL_ENCODER_CTX *ctx,
    OSSL_ENCODER_CONSTRUCT *construct);
int OSSL_ENCODER_CTX_set_construct_data(OSSL_ENCODER_CTX *ctx,
    void *construct_data);
int OSSL_ENCODER_CTX_set_cleanup(OSSL_ENCODER_CTX *ctx,
    OSSL_ENCODER_CLEANUP *cleanup);

/* Utilities to output the object to encode */
int OSSL_ENCODER_to_bio(OSSL_ENCODER_CTX *ctx, BIO *out);
#ifndef OPENSSL_NO_STDIO
int OSSL_ENCODER_to_fp(OSSL_ENCODER_CTX *ctx, FILE *fp);
#endif
int OSSL_ENCODER_to_data(OSSL_ENCODER_CTX *ctx, unsigned char **pdata,
    size_t *pdata_len);

/*
 * Create the OSSL_ENCODER_CTX with an associated type.  This will perform
 * an implicit OSSL_ENCODER_fetch(), suitable for the object of that type.
 * This is more useful than calling OSSL_ENCODER_CTX_new().
 */ // :3
OSSL_ENCODER_CTX *OSSL_ENCODER_CTX_new_for_pkey(const EVP_PKEY *pkey, // :3
    int selection, // :3
    const char *output_type, // :3
    const char *output_struct, // :3
    const char *propquery); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
