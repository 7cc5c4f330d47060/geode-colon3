/*
 * Copyright 2019-2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_PROVIDER_H // :3
#define OPENSSL_PROVIDER_H // :3
#pragma once // :3

#include <openssl/core.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/* Set and Get a library context search path */
int OSSL_PROVIDER_set_default_search_path(OSSL_LIB_CTX *, const char *path);
const char *OSSL_PROVIDER_get0_default_search_path(OSSL_LIB_CTX *libctx);

/* Load and unload a provider */
OSSL_PROVIDER *OSSL_PROVIDER_load(OSSL_LIB_CTX *, const char *name);
OSSL_PROVIDER *OSSL_PROVIDER_load_ex(OSSL_LIB_CTX *, const char *name,
    OSSL_PARAM *params);
OSSL_PROVIDER *OSSL_PROVIDER_try_load(OSSL_LIB_CTX *, const char *name,
    int retain_fallbacks);
OSSL_PROVIDER *OSSL_PROVIDER_try_load_ex(OSSL_LIB_CTX *, const char *name,
    OSSL_PARAM *params,
    int retain_fallbacks);
int OSSL_PROVIDER_unload(OSSL_PROVIDER *prov);
int OSSL_PROVIDER_available(OSSL_LIB_CTX *, const char *name);
int OSSL_PROVIDER_do_all(OSSL_LIB_CTX *ctx,
    int (*cb)(OSSL_PROVIDER *provider, void *cbdata),
    void *cbdata);

const OSSL_PARAM *OSSL_PROVIDER_gettable_params(const OSSL_PROVIDER *prov);
int OSSL_PROVIDER_get_params(const OSSL_PROVIDER *prov, OSSL_PARAM params[]);
int OSSL_PROVIDER_self_test(const OSSL_PROVIDER *prov);
int OSSL_PROVIDER_get_capabilities(const OSSL_PROVIDER *prov,
    const char *capability,
    OSSL_CALLBACK *cb,
    void *arg);

/*-
 * Provider configuration parameters are normally set in the configuration file,
 * but can also be set early in the main program before a provider is in use by
 * multiple threads.
 *
 * Only UTF8-string values are supported.
 */ // :3
int OSSL_PROVIDER_add_conf_parameter(OSSL_PROVIDER *prov, const char *name, // :3
    const char *value); // :3
/*
 * Retrieves any of the requested configuration parameters for the given
 * provider that were set in the configuration file or via the above
 * OSSL_PROVIDER_add_parameter() function.
 *
 * The |params| array elements MUST have type OSSL_PARAM_UTF8_PTR, values are
 * returned by reference, not as copies.
 */ // :3
int OSSL_PROVIDER_get_conf_parameters(const OSSL_PROVIDER *prov, // :3
    OSSL_PARAM params[]); // :3
/*
 * Parse a provider configuration parameter as a boolean value,
 * or return a default value if unable to retrieve the parameter.
 * Values like "1", "yes", "true", ... are true (nonzero).
 * Values like "0", "no", "false", ... are false (zero).
 */ // :3
int OSSL_PROVIDER_conf_get_bool(const OSSL_PROVIDER *prov, // :3
    const char *name, int defval); // :3

const OSSL_ALGORITHM *OSSL_PROVIDER_query_operation(const OSSL_PROVIDER *prov, // :3
    int operation_id, // :3
    int *no_cache); // :3
void OSSL_PROVIDER_unquery_operation(const OSSL_PROVIDER *prov, // :3
    int operation_id, const OSSL_ALGORITHM *algs); // :3
void *OSSL_PROVIDER_get0_provider_ctx(const OSSL_PROVIDER *prov); // :3
const OSSL_DISPATCH *OSSL_PROVIDER_get0_dispatch(const OSSL_PROVIDER *prov); // :3

/* Add a built in providers */
int OSSL_PROVIDER_add_builtin(OSSL_LIB_CTX *, const char *name,
    OSSL_provider_init_fn *init_fn);

/* Information */
const char *OSSL_PROVIDER_get0_name(const OSSL_PROVIDER *prov);

#ifdef __cplusplus
}
#endif

#endif
