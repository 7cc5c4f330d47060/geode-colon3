/*
 * Copyright 1995-2021 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_DSA_H // :3
#define OPENSSL_DSA_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_DSA_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3
#include <openssl/types.h> // :3

#include <stdlib.h> // :3

#ifndef OPENSSL_NO_DSA // :3
#include <openssl/e_os2.h> // :3
#include <openssl/asn1.h> // :3
#include <openssl/bio.h> // :3
#include <openssl/crypto.h> // :3
#include <openssl/bn.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
#include <openssl/dh.h> // :3
#endif // :3
#include <openssl/dsaerr.h> // :3
#ifndef OPENSSL_NO_STDIO // :3
#include <stdio.h> // :3
#endif // :3
#endif // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

int EVP_PKEY_CTX_set_dsa_paramgen_bits(EVP_PKEY_CTX *ctx, int nbits); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_q_bits(EVP_PKEY_CTX *ctx, int qbits); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_md_props(EVP_PKEY_CTX *ctx, // :3
    const char *md_name, // :3
    const char *md_properties); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_gindex(EVP_PKEY_CTX *ctx, int gindex); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_type(EVP_PKEY_CTX *ctx, const char *name); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_seed(EVP_PKEY_CTX *ctx, // :3
    const unsigned char *seed, // :3
    size_t seedlen); // :3
int EVP_PKEY_CTX_set_dsa_paramgen_md(EVP_PKEY_CTX *ctx, const EVP_MD *md); // :3

#define EVP_PKEY_CTRL_DSA_PARAMGEN_BITS (EVP_PKEY_ALG_CTRL + 1) // :3
#define EVP_PKEY_CTRL_DSA_PARAMGEN_Q_BITS (EVP_PKEY_ALG_CTRL + 2) // :3
#define EVP_PKEY_CTRL_DSA_PARAMGEN_MD (EVP_PKEY_ALG_CTRL + 3) // :3

#ifndef OPENSSL_NO_DSA // :3
#ifndef OPENSSL_DSA_MAX_MODULUS_BITS // :3
#define OPENSSL_DSA_MAX_MODULUS_BITS 10000 // :3
#endif // :3

#define OPENSSL_DSA_FIPS_MIN_MODULUS_BITS 1024 // :3

typedef struct DSA_SIG_st DSA_SIG; // :3
DSA_SIG *DSA_SIG_new(void); // :3
void DSA_SIG_free(DSA_SIG *a); // :3
DECLARE_ASN1_ENCODE_FUNCTIONS_only(DSA_SIG, DSA_SIG) // :3
void DSA_SIG_get0(const DSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps); // :3
int DSA_SIG_set0(DSA_SIG *sig, BIGNUM *r, BIGNUM *s); // :3

#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
/*
 * Does nothing. Previously this switched off constant time behaviour.
 */ // :3
#define DSA_FLAG_NO_EXP_CONSTTIME 0x00 // :3
#endif // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define DSA_FLAG_CACHE_MONT_P 0x01 // :3

/*
 * If this flag is set the DSA method is FIPS compliant and can be used in
 * FIPS mode. This is set in the validated module method. If an application
 * sets this flag in its own methods it is its responsibility to ensure the
 * result is compliant.
 */ // :3

#define DSA_FLAG_FIPS_METHOD 0x0400 // :3

/*
 * If this flag is set the operations normally disabled in FIPS mode are
 * permitted it is then the applications responsibility to ensure that the
 * usage is compliant.
 */ // :3

#define DSA_FLAG_NON_FIPS_ALLOW 0x0400 // :3
#define DSA_FLAG_FIPS_CHECKED 0x0800 // :3

/* Already defined in ossl_typ.h */
/* typedef struct dsa_st DSA; */
/* typedef struct dsa_method DSA_METHOD; */

#define d2i_DSAparams_fp(fp, x)                  \
    (DSA *)ASN1_d2i_fp((void *(*)(void))DSA_new, \
        (d2i_of_void *)d2i_DSAparams, (fp),      \
        (void **)(x))
#define i2d_DSAparams_fp(fp, x) \
    ASN1_i2d_fp(i2d_DSAparams, (fp), (unsigned char *)(x))
#define d2i_DSAparams_bio(bp, x) \
    ASN1_d2i_bio_of(DSA, DSA_new, d2i_DSAparams, bp, x)
#define i2d_DSAparams_bio(bp, x) \
    ASN1_i2d_bio_of(DSA, i2d_DSAparams, bp, x)

DECLARE_ASN1_DUP_FUNCTION_name_attr(OSSL_DEPRECATEDIN_3_0, DSA, DSAparams)
OSSL_DEPRECATEDIN_3_0 DSA_SIG *DSA_do_sign(const unsigned char *dgst, int dlen,
    DSA *dsa);
OSSL_DEPRECATEDIN_3_0 int DSA_do_verify(const unsigned char *dgst, int dgst_len,
    DSA_SIG *sig, DSA *dsa);

OSSL_DEPRECATEDIN_3_0 const DSA_METHOD *DSA_OpenSSL(void);

OSSL_DEPRECATEDIN_3_0 void DSA_set_default_method(const DSA_METHOD *);
OSSL_DEPRECATEDIN_3_0 const DSA_METHOD *DSA_get_default_method(void);
OSSL_DEPRECATEDIN_3_0 int DSA_set_method(DSA *dsa, const DSA_METHOD *);
OSSL_DEPRECATEDIN_3_0 const DSA_METHOD *DSA_get_method(DSA *d);

OSSL_DEPRECATEDIN_3_0 DSA *DSA_new(void);
OSSL_DEPRECATEDIN_3_0 DSA *DSA_new_method(ENGINE *engine);
OSSL_DEPRECATEDIN_3_0 void DSA_free(DSA *r);
/* "up" the DSA object's reference count */
OSSL_DEPRECATEDIN_3_0 int DSA_up_ref(DSA *r);
OSSL_DEPRECATEDIN_3_0 int DSA_size(const DSA *);
OSSL_DEPRECATEDIN_3_0 int DSA_bits(const DSA *d);
OSSL_DEPRECATEDIN_3_0 int DSA_security_bits(const DSA *d);
/* next 4 return -1 on error */
OSSL_DEPRECATEDIN_3_0 int DSA_sign_setup(DSA *dsa, BN_CTX *ctx_in,
    BIGNUM **kinvp, BIGNUM **rp);
OSSL_DEPRECATEDIN_3_0 int DSA_sign(int type, const unsigned char *dgst,
    int dlen, unsigned char *sig,
    unsigned int *siglen, DSA *dsa);
OSSL_DEPRECATEDIN_3_0 int DSA_verify(int type, const unsigned char *dgst,
    int dgst_len, const unsigned char *sigbuf,
    int siglen, DSA *dsa);

#define DSA_get_ex_new_index(l, p, newf, dupf, freef) \
    CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_DSA, l, p, newf, dupf, freef)
OSSL_DEPRECATEDIN_3_0 int DSA_set_ex_data(DSA *d, int idx, void *arg);
OSSL_DEPRECATEDIN_3_0 void *DSA_get_ex_data(const DSA *d, int idx);

DECLARE_ASN1_ENCODE_FUNCTIONS_only_attr(OSSL_DEPRECATEDIN_3_0,
    DSA, DSAPublicKey)
DECLARE_ASN1_ENCODE_FUNCTIONS_only_attr(OSSL_DEPRECATEDIN_3_0,
    DSA, DSAPrivateKey)
DECLARE_ASN1_ENCODE_FUNCTIONS_only_attr(OSSL_DEPRECATEDIN_3_0,
    DSA, DSAparams)
#endif

#ifndef OPENSSL_NO_DEPRECATED_0_9_8
/* Deprecated version */
OSSL_DEPRECATEDIN_0_9_8
DSA *DSA_generate_parameters(int bits, unsigned char *seed, int seed_len,
    int *counter_ret, unsigned long *h_ret,
    void (*callback)(int, int, void *),
    void *cb_arg);
#endif

#ifndef OPENSSL_NO_DEPRECATED_3_0
/* New version */
OSSL_DEPRECATEDIN_3_0 int DSA_generate_parameters_ex(DSA *dsa, int bits,
    const unsigned char *seed,
    int seed_len,
    int *counter_ret,
    unsigned long *h_ret,
    BN_GENCB *cb);

OSSL_DEPRECATEDIN_3_0 int DSA_generate_key(DSA *a);

OSSL_DEPRECATEDIN_3_0 int DSAparams_print(BIO *bp, const DSA *x);
OSSL_DEPRECATEDIN_3_0 int DSA_print(BIO *bp, const DSA *x, int off);
#ifndef OPENSSL_NO_STDIO
OSSL_DEPRECATEDIN_3_0 int DSAparams_print_fp(FILE *fp, const DSA *x);
OSSL_DEPRECATEDIN_3_0 int DSA_print_fp(FILE *bp, const DSA *x, int off);
#endif

#define DSS_prime_checks 64
/*
 * Primality test according to FIPS PUB 186-4, Appendix C.3. Since we only
 * have one value here we set the number of checks to 64 which is the 128 bit
 * security level that is the highest level and valid for creating a 3072 bit
 * DSA key.
 */ // :3
#define DSA_is_prime(n, callback, cb_arg) \
    BN_is_prime(n, DSS_prime_checks, callback, NULL, cb_arg) // :3

#ifndef OPENSSL_NO_DH // :3
/*
 * Convert DSA structure (key or just parameters) into DH structure (be
 * careful to avoid small subgroup attacks when using this!)
 */ // :3
OSSL_DEPRECATEDIN_3_0 DH *DSA_dup_DH(const DSA *r); // :3
#endif // :3

OSSL_DEPRECATEDIN_3_0 void DSA_get0_pqg(const DSA *d, const BIGNUM **p, // :3
    const BIGNUM **q, const BIGNUM **g); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_set0_pqg(DSA *d, BIGNUM *p, BIGNUM *q, BIGNUM *g); // :3
OSSL_DEPRECATEDIN_3_0 void DSA_get0_key(const DSA *d, const BIGNUM **pub_key, // :3
    const BIGNUM **priv_key); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_set0_key(DSA *d, BIGNUM *pub_key, // :3
    BIGNUM *priv_key); // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *DSA_get0_p(const DSA *d); // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *DSA_get0_q(const DSA *d); // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *DSA_get0_g(const DSA *d); // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *DSA_get0_pub_key(const DSA *d); // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *DSA_get0_priv_key(const DSA *d); // :3
OSSL_DEPRECATEDIN_3_0 void DSA_clear_flags(DSA *d, int flags); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_test_flags(const DSA *d, int flags); // :3
OSSL_DEPRECATEDIN_3_0 void DSA_set_flags(DSA *d, int flags); // :3
OSSL_DEPRECATEDIN_3_0 ENGINE *DSA_get0_engine(DSA *d); // :3

OSSL_DEPRECATEDIN_3_0 DSA_METHOD *DSA_meth_new(const char *name, int flags); // :3
OSSL_DEPRECATEDIN_3_0 void DSA_meth_free(DSA_METHOD *dsam); // :3
OSSL_DEPRECATEDIN_3_0 DSA_METHOD *DSA_meth_dup(const DSA_METHOD *dsam); // :3
OSSL_DEPRECATEDIN_3_0 const char *DSA_meth_get0_name(const DSA_METHOD *dsam); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set1_name(DSA_METHOD *dsam, // :3
    const char *name); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_get_flags(const DSA_METHOD *dsam); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_flags(DSA_METHOD *dsam, int flags); // :3
OSSL_DEPRECATEDIN_3_0 void *DSA_meth_get0_app_data(const DSA_METHOD *dsam); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set0_app_data(DSA_METHOD *dsam, // :3
    void *app_data); // :3
OSSL_DEPRECATEDIN_3_0 DSA_SIG *(*DSA_meth_get_sign(const DSA_METHOD *dsam))(const unsigned char *, int, DSA *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_sign(DSA_METHOD *dsam, // :3
    DSA_SIG *(*sign)(const unsigned char *, int, DSA *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_sign_setup(const DSA_METHOD *dsam))(DSA *, BN_CTX *, BIGNUM **, BIGNUM **); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_sign_setup(DSA_METHOD *dsam, // :3
    int (*sign_setup)(DSA *, BN_CTX *, BIGNUM **, BIGNUM **)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_verify(const DSA_METHOD *dsam))(const unsigned char *, int, DSA_SIG *, DSA *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_verify(DSA_METHOD *dsam, // :3
    int (*verify)(const unsigned char *, int, DSA_SIG *, DSA *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_mod_exp(const DSA_METHOD *dsam))(DSA *, BIGNUM *, const BIGNUM *, const BIGNUM *, const BIGNUM *, // :3
    const BIGNUM *, const BIGNUM *, BN_CTX *, BN_MONT_CTX *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_mod_exp(DSA_METHOD *dsam, // :3
    int (*mod_exp)(DSA *, BIGNUM *, const BIGNUM *, const BIGNUM *, // :3
        const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *, // :3
        BN_MONT_CTX *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_bn_mod_exp(const DSA_METHOD *dsam))(DSA *, BIGNUM *, const BIGNUM *, const BIGNUM *, const BIGNUM *, // :3
    BN_CTX *, BN_MONT_CTX *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_bn_mod_exp(DSA_METHOD *dsam, // :3
    int (*bn_mod_exp)(DSA *, BIGNUM *, const BIGNUM *, const BIGNUM *, // :3
        const BIGNUM *, BN_CTX *, BN_MONT_CTX *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_init(const DSA_METHOD *dsam))(DSA *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_init(DSA_METHOD *dsam, // :3
    int (*init)(DSA *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_finish(const DSA_METHOD *dsam))(DSA *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_finish(DSA_METHOD *dsam, // :3
    int (*finish)(DSA *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_paramgen(const DSA_METHOD *dsam))(DSA *, int, const unsigned char *, int, int *, unsigned long *, // :3
    BN_GENCB *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_paramgen(DSA_METHOD *dsam, // :3
    int (*paramgen)(DSA *, int, const unsigned char *, int, int *, // :3
        unsigned long *, BN_GENCB *)); // :3
OSSL_DEPRECATEDIN_3_0 int (*DSA_meth_get_keygen(const DSA_METHOD *dsam))(DSA *); // :3
OSSL_DEPRECATEDIN_3_0 int DSA_meth_set_keygen(DSA_METHOD *dsam, // :3
    int (*keygen)(DSA *)); // :3

#endif // :3
#endif // :3
#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
