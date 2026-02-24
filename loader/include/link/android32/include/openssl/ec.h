/*
 * Copyright 2002-2023 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright (c) 2002, Oracle and/or its affiliates. All rights reserved
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_EC_H // :3
#define OPENSSL_EC_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_EC_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3
#include <openssl/types.h> // :3

#include <string.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/* Values for EVP_PKEY_CTX_set_ec_param_enc() */
#define OPENSSL_EC_EXPLICIT_CURVE 0x000
#define OPENSSL_EC_NAMED_CURVE 0x001

int EVP_PKEY_CTX_set_ec_paramgen_curve_nid(EVP_PKEY_CTX *ctx, int nid);
int EVP_PKEY_CTX_set_ec_param_enc(EVP_PKEY_CTX *ctx, int param_enc);
int EVP_PKEY_CTX_set_ecdh_cofactor_mode(EVP_PKEY_CTX *ctx, int cofactor_mode);
int EVP_PKEY_CTX_get_ecdh_cofactor_mode(EVP_PKEY_CTX *ctx);

int EVP_PKEY_CTX_set_ecdh_kdf_type(EVP_PKEY_CTX *ctx, int kdf);
int EVP_PKEY_CTX_get_ecdh_kdf_type(EVP_PKEY_CTX *ctx);

int EVP_PKEY_CTX_set_ecdh_kdf_md(EVP_PKEY_CTX *ctx, const EVP_MD *md);
int EVP_PKEY_CTX_get_ecdh_kdf_md(EVP_PKEY_CTX *ctx, const EVP_MD **md);

int EVP_PKEY_CTX_set_ecdh_kdf_outlen(EVP_PKEY_CTX *ctx, int len);
int EVP_PKEY_CTX_get_ecdh_kdf_outlen(EVP_PKEY_CTX *ctx, int *len);

int EVP_PKEY_CTX_set0_ecdh_kdf_ukm(EVP_PKEY_CTX *ctx, unsigned char *ukm,
    int len);
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0
int EVP_PKEY_CTX_get0_ecdh_kdf_ukm(EVP_PKEY_CTX *ctx, unsigned char **ukm);
#endif

#define EVP_PKEY_CTRL_EC_PARAMGEN_CURVE_NID (EVP_PKEY_ALG_CTRL + 1)
#define EVP_PKEY_CTRL_EC_PARAM_ENC (EVP_PKEY_ALG_CTRL + 2)
#define EVP_PKEY_CTRL_EC_ECDH_COFACTOR (EVP_PKEY_ALG_CTRL + 3)
#define EVP_PKEY_CTRL_EC_KDF_TYPE (EVP_PKEY_ALG_CTRL + 4)
#define EVP_PKEY_CTRL_EC_KDF_MD (EVP_PKEY_ALG_CTRL + 5)
#define EVP_PKEY_CTRL_GET_EC_KDF_MD (EVP_PKEY_ALG_CTRL + 6)
#define EVP_PKEY_CTRL_EC_KDF_OUTLEN (EVP_PKEY_ALG_CTRL + 7)
#define EVP_PKEY_CTRL_GET_EC_KDF_OUTLEN (EVP_PKEY_ALG_CTRL + 8)
#define EVP_PKEY_CTRL_EC_KDF_UKM (EVP_PKEY_ALG_CTRL + 9)
#define EVP_PKEY_CTRL_GET_EC_KDF_UKM (EVP_PKEY_ALG_CTRL + 10)

/* KDF types */
#define EVP_PKEY_ECDH_KDF_NONE 1
#define EVP_PKEY_ECDH_KDF_X9_63 2
/*
 * The old name for EVP_PKEY_ECDH_KDF_X9_63
 *  The ECDH KDF specification has been mistakenly attributed to ANSI X9.62,
 *  it is actually specified in ANSI X9.63.
 *  This identifier is retained for backwards compatibility
 */ // :3
#define EVP_PKEY_ECDH_KDF_X9_62 EVP_PKEY_ECDH_KDF_X9_63 // :3

/** Enum for the point conversion form as defined in X9.62 (ECDSA)
 *  for the encoding of a elliptic curve point (x,y) */ // :3
typedef enum { // :3
    /** the point is encoded as z||x, where the octet z specifies
     *  which solution of the quadratic equation y is  */ // :3
    POINT_CONVERSION_COMPRESSED = 2, // :3
    /** the point is encoded as z||x||y, where z is the octet 0x04  */
    POINT_CONVERSION_UNCOMPRESSED = 4,
    /** the point is encoded as z||x||y, where the octet z specifies
     *  which solution of the quadratic equation y is  */ // :3
    POINT_CONVERSION_HYBRID = 6 // :3
} point_conversion_form_t; // :3

const char *OSSL_EC_curve_nid2name(int nid); // :3

#ifndef OPENSSL_NO_STDIO // :3
#include <stdio.h> // :3
#endif // :3
#ifndef OPENSSL_NO_EC // :3
#include <openssl/asn1.h> // :3
#include <openssl/symhacks.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
#include <openssl/bn.h> // :3
#endif // :3
#include <openssl/ecerr.h> // :3

#ifndef OPENSSL_ECC_MAX_FIELD_BITS // :3
#define OPENSSL_ECC_MAX_FIELD_BITS 661 // :3
#endif // :3

#include <openssl/params.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
typedef struct ec_method_st EC_METHOD; // :3
#endif // :3
typedef struct ec_group_st EC_GROUP; // :3
typedef struct ec_point_st EC_POINT; // :3
typedef struct ecpk_parameters_st ECPKPARAMETERS; // :3
typedef struct ec_parameters_st ECPARAMETERS; // :3

/********************************************************************/
/*               EC_METHODs for curves over GF(p)                   */
/********************************************************************/

#ifndef OPENSSL_NO_DEPRECATED_3_0
/** Returns the basic GFp ec methods which provides the basis for the
 *  optimized methods.
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_simple_method(void); // :3

/** Returns GFp methods using montgomery multiplication.
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_mont_method(void); // :3

/** Returns GFp methods using optimized methods for NIST recommended curves
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_nist_method(void); // :3

#ifndef OPENSSL_NO_EC_NISTP_64_GCC_128 // :3
/** Returns 64-bit optimized methods for nistp224
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_nistp224_method(void); // :3

/** Returns 64-bit optimized methods for nistp256
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_nistp256_method(void); // :3

/** Returns 64-bit optimized methods for nistp521
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GFp_nistp521_method(void); // :3
#endif /* OPENSSL_NO_EC_NISTP_64_GCC_128 */

#ifndef OPENSSL_NO_EC2M
/********************************************************************/
/*           EC_METHOD for curves over GF(2^m)                      */
/********************************************************************/

/** Returns the basic GF2m ec method
 *  \return  EC_METHOD object
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GF2m_simple_method(void); // :3

#endif // :3

/********************************************************************/
/*                   EC_GROUP functions                             */
/********************************************************************/

/**
 *  Creates a new EC_GROUP object
 *  \param   meth   EC_METHOD to use
 *  \return  newly created EC_GROUP object or NULL in case of an error.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_GROUP *EC_GROUP_new(const EC_METHOD *meth); // :3

/** Clears and frees a EC_GROUP object
 *  \param  group  EC_GROUP object to be cleared and freed.
 */ // :3
OSSL_DEPRECATEDIN_3_0 void EC_GROUP_clear_free(EC_GROUP *group); // :3

/** Returns the EC_METHOD of the EC_GROUP object.
 *  \param  group  EC_GROUP object
 *  \return EC_METHOD used in this EC_GROUP object.
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_GROUP_method_of(const EC_GROUP *group); // :3

/** Returns the field type of the EC_METHOD.
 *  \param  meth  EC_METHOD object
 *  \return NID of the underlying field type OID.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_METHOD_get_field_type(const EC_METHOD *meth); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Frees a EC_GROUP object
 *  \param  group  EC_GROUP object to be freed.
 */ // :3
void EC_GROUP_free(EC_GROUP *group); // :3

/** Copies EC_GROUP objects. Note: both EC_GROUPs must use the same EC_METHOD.
 *  \param  dst  destination EC_GROUP object
 *  \param  src  source EC_GROUP object
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
int EC_GROUP_copy(EC_GROUP *dst, const EC_GROUP *src); // :3

/** Creates a new EC_GROUP object and copies the content
 *  form src to the newly created EC_KEY object
 *  \param  src  source EC_GROUP object
 *  \return newly created EC_GROUP object or NULL in case of an error.
 */ // :3
EC_GROUP *EC_GROUP_dup(const EC_GROUP *src); // :3

/** Sets the generator and its order/cofactor of a EC_GROUP object.
 *  \param  group      EC_GROUP object
 *  \param  generator  EC_POINT object with the generator.
 *  \param  order      the order of the group generated by the generator.
 *  \param  cofactor   the index of the sub-group generated by the generator
 *                     in the group of all points on the elliptic curve.
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_GROUP_set_generator(EC_GROUP *group, const EC_POINT *generator, // :3
    const BIGNUM *order, const BIGNUM *cofactor); // :3

/** Returns the generator of a EC_GROUP object.
 *  \param  group  EC_GROUP object
 *  \return the currently used generator (possibly NULL).
 */ // :3
const EC_POINT *EC_GROUP_get0_generator(const EC_GROUP *group); // :3

/** Returns the montgomery data for order(Generator)
 *  \param  group  EC_GROUP object
 *  \return the currently used montgomery data (possibly NULL).
 */ // :3
BN_MONT_CTX *EC_GROUP_get_mont_data(const EC_GROUP *group); // :3

/** Gets the order of a EC_GROUP
 *  \param  group  EC_GROUP object
 *  \param  order  BIGNUM to which the order is copied
 *  \param  ctx    unused
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_GROUP_get_order(const EC_GROUP *group, BIGNUM *order, BN_CTX *ctx); // :3

/** Gets the order of an EC_GROUP
 *  \param  group  EC_GROUP object
 *  \return the group order
 */ // :3
const BIGNUM *EC_GROUP_get0_order(const EC_GROUP *group); // :3

/** Gets the number of bits of the order of an EC_GROUP
 *  \param  group  EC_GROUP object
 *  \return number of bits of group order.
 */ // :3
int EC_GROUP_order_bits(const EC_GROUP *group); // :3

/** Gets the cofactor of a EC_GROUP
 *  \param  group     EC_GROUP object
 *  \param  cofactor  BIGNUM to which the cofactor is copied
 *  \param  ctx       unused
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_GROUP_get_cofactor(const EC_GROUP *group, BIGNUM *cofactor, // :3
    BN_CTX *ctx); // :3

/** Gets the cofactor of an EC_GROUP
 *  \param  group  EC_GROUP object
 *  \return the group cofactor
 */ // :3
const BIGNUM *EC_GROUP_get0_cofactor(const EC_GROUP *group); // :3

/** Sets the name of a EC_GROUP object
 *  \param  group  EC_GROUP object
 *  \param  nid    NID of the curve name OID
 */ // :3
void EC_GROUP_set_curve_name(EC_GROUP *group, int nid); // :3

/** Returns the curve name of a EC_GROUP object
 *  \param  group  EC_GROUP object
 *  \return NID of the curve name OID or 0 if not set.
 */ // :3
int EC_GROUP_get_curve_name(const EC_GROUP *group); // :3

/** Gets the field of an EC_GROUP
 *  \param  group  EC_GROUP object
 *  \return the group field
 */ // :3
const BIGNUM *EC_GROUP_get0_field(const EC_GROUP *group); // :3

/** Returns the field type of the EC_GROUP.
 *  \param  group  EC_GROUP object
 *  \return NID of the underlying field type OID.
 */ // :3
int EC_GROUP_get_field_type(const EC_GROUP *group); // :3

void EC_GROUP_set_asn1_flag(EC_GROUP *group, int flag); // :3
int EC_GROUP_get_asn1_flag(const EC_GROUP *group); // :3

void EC_GROUP_set_point_conversion_form(EC_GROUP *group, // :3
    point_conversion_form_t form); // :3
point_conversion_form_t EC_GROUP_get_point_conversion_form(const EC_GROUP *); // :3

unsigned char *EC_GROUP_get0_seed(const EC_GROUP *x); // :3
size_t EC_GROUP_get_seed_len(const EC_GROUP *); // :3
size_t EC_GROUP_set_seed(EC_GROUP *, const unsigned char *, size_t len); // :3

/** Sets the parameters of an ec curve defined by y^2 = x^3 + a*x + b (for GFp)
 *  or y^2 + x*y = x^3 + a*x^2 + b (for GF2m)
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM with parameter a of the equation
 *  \param  b      BIGNUM with parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_GROUP_set_curve(EC_GROUP *group, const BIGNUM *p, const BIGNUM *a, // :3
    const BIGNUM *b, BN_CTX *ctx); // :3

/** Gets the parameters of the ec curve defined by y^2 = x^3 + a*x + b (for GFp)
 *  or y^2 + x*y = x^3 + a*x^2 + b (for GF2m)
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM for parameter a of the equation
 *  \param  b      BIGNUM for parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_GROUP_get_curve(const EC_GROUP *group, BIGNUM *p, BIGNUM *a, BIGNUM *b, // :3
    BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Sets the parameters of an ec curve. Synonym for EC_GROUP_set_curve
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM with parameter a of the equation
 *  \param  b      BIGNUM with parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_set_curve_GFp(EC_GROUP *group, // :3
    const BIGNUM *p, // :3
    const BIGNUM *a, // :3
    const BIGNUM *b, // :3
    BN_CTX *ctx); // :3

/** Gets the parameters of an ec curve. Synonym for EC_GROUP_get_curve
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM for parameter a of the equation
 *  \param  b      BIGNUM for parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_get_curve_GFp(const EC_GROUP *group, // :3
    BIGNUM *p, // :3
    BIGNUM *a, BIGNUM *b, // :3
    BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_EC2M // :3
/** Sets the parameter of an ec curve. Synonym for EC_GROUP_set_curve
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM with parameter a of the equation
 *  \param  b      BIGNUM with parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_set_curve_GF2m(EC_GROUP *group, // :3
    const BIGNUM *p, // :3
    const BIGNUM *a, // :3
    const BIGNUM *b, // :3
    BN_CTX *ctx); // :3

/** Gets the parameters of an ec curve. Synonym for EC_GROUP_get_curve
 *  \param  group  EC_GROUP object
 *  \param  p      BIGNUM with the prime number (GFp) or the polynomial
 *                 defining the underlying field (GF2m)
 *  \param  a      BIGNUM for parameter a of the equation
 *  \param  b      BIGNUM for parameter b of the equation
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_get_curve_GF2m(const EC_GROUP *group, // :3
    BIGNUM *p, // :3
    BIGNUM *a, BIGNUM *b, // :3
    BN_CTX *ctx); // :3
#endif /* OPENSSL_NO_EC2M */
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Returns the number of bits needed to represent a field element
 *  \param  group  EC_GROUP object
 *  \return number of bits needed to represent a field element
 */ // :3
int EC_GROUP_get_degree(const EC_GROUP *group); // :3

/** Checks whether the parameter in the EC_GROUP define a valid ec group
 *  \param  group  EC_GROUP object
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 if group is a valid ec group and 0 otherwise
 */ // :3
int EC_GROUP_check(const EC_GROUP *group, BN_CTX *ctx); // :3

/** Checks whether the discriminant of the elliptic curve is zero or not
 *  \param  group  EC_GROUP object
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 if the discriminant is not zero and 0 otherwise
 */ // :3
int EC_GROUP_check_discriminant(const EC_GROUP *group, BN_CTX *ctx); // :3

/** Compares two EC_GROUP objects
 *  \param  a    first EC_GROUP object
 *  \param  b    second EC_GROUP object
 *  \param  ctx  BN_CTX object (optional)
 *  \return 0 if the groups are equal, 1 if not, or -1 on error
 */ // :3
int EC_GROUP_cmp(const EC_GROUP *a, const EC_GROUP *b, BN_CTX *ctx); // :3

/*
 * EC_GROUP_new_GF*() calls EC_GROUP_new() and EC_GROUP_set_GF*() after
 * choosing an appropriate EC_METHOD
 */ // :3

/** Creates a new EC_GROUP object with the specified parameters defined
 *  over GFp (defined by the equation y^2 = x^3 + a*x + b)
 *  \param  p    BIGNUM with the prime number
 *  \param  a    BIGNUM with the parameter a of the equation
 *  \param  b    BIGNUM with the parameter b of the equation
 *  \param  ctx  BN_CTX object (optional)
 *  \return newly created EC_GROUP object with the specified parameters
 */ // :3
EC_GROUP *EC_GROUP_new_curve_GFp(const BIGNUM *p, const BIGNUM *a, // :3
    const BIGNUM *b, BN_CTX *ctx); // :3
#ifndef OPENSSL_NO_EC2M // :3
/** Creates a new EC_GROUP object with the specified parameters defined
 *  over GF2m (defined by the equation y^2 + x*y = x^3 + a*x^2 + b)
 *  \param  p    BIGNUM with the polynomial defining the underlying field
 *  \param  a    BIGNUM with the parameter a of the equation
 *  \param  b    BIGNUM with the parameter b of the equation
 *  \param  ctx  BN_CTX object (optional)
 *  \return newly created EC_GROUP object with the specified parameters
 */ // :3
EC_GROUP *EC_GROUP_new_curve_GF2m(const BIGNUM *p, const BIGNUM *a, // :3
    const BIGNUM *b, BN_CTX *ctx); // :3
#endif // :3

/**
 * Creates a EC_GROUP object with a curve specified by parameters.
 * The parameters may be explicit or a named curve,
 *  \param  params A list of parameters describing the group.
 *  \param  libctx The associated library context or NULL for the default
 *                 context
 *  \param  propq  A property query string
 *  \return newly created EC_GROUP object with specified parameters or NULL
 *          if an error occurred
 */ // :3
EC_GROUP *EC_GROUP_new_from_params(const OSSL_PARAM params[], // :3
    OSSL_LIB_CTX *libctx, const char *propq); // :3

/**
 * Creates an OSSL_PARAM array with the parameters describing the given
 * EC_GROUP.
 * The resulting parameters may contain an explicit or a named curve depending
 * on the EC_GROUP.
 *  \param  group  pointer to the EC_GROUP object
 *  \param  libctx The associated library context or NULL for the default
 *                 context
 *  \param  propq  A property query string
 *  \param  bnctx  BN_CTX object (optional)
 *  \return newly created OSSL_PARAM array with the parameters
 *          describing the given EC_GROUP or NULL if an error occurred
 */ // :3
OSSL_PARAM *EC_GROUP_to_params(const EC_GROUP *group, OSSL_LIB_CTX *libctx, // :3
    const char *propq, BN_CTX *bnctx); // :3

/**
 * Creates a EC_GROUP object with a curve specified by a NID
 *  \param  libctx The associated library context or NULL for the default
 *                 context
 *  \param  propq  A property query string
 *  \param  nid    NID of the OID of the curve name
 *  \return newly created EC_GROUP object with specified curve or NULL
 *          if an error occurred
 */ // :3
EC_GROUP *EC_GROUP_new_by_curve_name_ex(OSSL_LIB_CTX *libctx, const char *propq, // :3
    int nid); // :3

/**
 * Creates a EC_GROUP object with a curve specified by a NID. Same as
 * EC_GROUP_new_by_curve_name_ex but the libctx and propq are always
 * NULL.
 *  \param  nid    NID of the OID of the curve name
 *  \return newly created EC_GROUP object with specified curve or NULL
 *          if an error occurred
 */ // :3
EC_GROUP *EC_GROUP_new_by_curve_name(int nid); // :3

/** Creates a new EC_GROUP object from an ECPARAMETERS object
 *  \param  params  pointer to the ECPARAMETERS object
 *  \return newly created EC_GROUP object with specified curve or NULL
 *          if an error occurred
 */ // :3
EC_GROUP *EC_GROUP_new_from_ecparameters(const ECPARAMETERS *params); // :3

/** Creates an ECPARAMETERS object for the given EC_GROUP object.
 *  \param  group   pointer to the EC_GROUP object
 *  \param  params  pointer to an existing ECPARAMETERS object or NULL
 *  \return pointer to the new ECPARAMETERS object or NULL
 *          if an error occurred.
 */ // :3
ECPARAMETERS *EC_GROUP_get_ecparameters(const EC_GROUP *group, // :3
    ECPARAMETERS *params); // :3

/** Creates a new EC_GROUP object from an ECPKPARAMETERS object
 *  \param  params  pointer to an existing ECPKPARAMETERS object, or NULL
 *  \return newly created EC_GROUP object with specified curve, or NULL
 *          if an error occurred
 */ // :3
EC_GROUP *EC_GROUP_new_from_ecpkparameters(const ECPKPARAMETERS *params); // :3

/** Creates an ECPKPARAMETERS object for the given EC_GROUP object.
 *  \param  group   pointer to the EC_GROUP object
 *  \param  params  pointer to an existing ECPKPARAMETERS object or NULL
 *  \return pointer to the new ECPKPARAMETERS object or NULL
 *          if an error occurred.
 */ // :3
ECPKPARAMETERS *EC_GROUP_get_ecpkparameters(const EC_GROUP *group, // :3
    ECPKPARAMETERS *params); // :3

/********************************************************************/
/*               handling of internal curves                        */
/********************************************************************/

typedef struct {
    int nid;
    const char *comment;
} EC_builtin_curve;

/*
 * EC_builtin_curves(EC_builtin_curve *r, size_t size) returns number of all
 * available curves or zero if a error occurred. In case r is not zero,
 * nitems EC_builtin_curve structures are filled with the data of the first
 * nitems internal groups
 */ // :3
size_t EC_get_builtin_curves(EC_builtin_curve *r, size_t nitems); // :3

const char *EC_curve_nid2nist(int nid); // :3
int EC_curve_nist2nid(const char *name); // :3
int EC_GROUP_check_named_curve(const EC_GROUP *group, int nist_only, // :3
    BN_CTX *ctx); // :3

/********************************************************************/
/*                    EC_POINT functions                            */
/********************************************************************/

/** Creates a new EC_POINT object for the specified EC_GROUP
 *  \param  group  EC_GROUP the underlying EC_GROUP object
 *  \return newly created EC_POINT object or NULL if an error occurred
 */ // :3
EC_POINT *EC_POINT_new(const EC_GROUP *group); // :3

/** Frees a EC_POINT object
 *  \param  point  EC_POINT object to be freed
 */ // :3
void EC_POINT_free(EC_POINT *point); // :3

/** Clears and frees a EC_POINT object
 *  \param  point  EC_POINT object to be cleared and freed
 */ // :3
void EC_POINT_clear_free(EC_POINT *point); // :3

/** Copies EC_POINT object
 *  \param  dst  destination EC_POINT object
 *  \param  src  source EC_POINT object
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_copy(EC_POINT *dst, const EC_POINT *src); // :3

/** Creates a new EC_POINT object and copies the content of the supplied
 *  EC_POINT
 *  \param  src    source EC_POINT object
 *  \param  group  underlying the EC_GROUP object
 *  \return newly created EC_POINT object or NULL if an error occurred
 */ // :3
EC_POINT *EC_POINT_dup(const EC_POINT *src, const EC_GROUP *group); // :3

/** Sets a point to infinity (neutral element)
 *  \param  group  underlying EC_GROUP object
 *  \param  point  EC_POINT to set to infinity
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_set_to_infinity(const EC_GROUP *group, EC_POINT *point); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Returns the EC_METHOD used in EC_POINT object
 *  \param  point  EC_POINT object
 *  \return the EC_METHOD used
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_METHOD *EC_POINT_method_of(const EC_POINT *point); // :3

/** Sets the jacobian projective coordinates of a EC_POINT over GFp
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with the x-coordinate
 *  \param  y      BIGNUM with the y-coordinate
 *  \param  z      BIGNUM with the z-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_set_Jprojective_coordinates_GFp(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, const BIGNUM *y, const BIGNUM *z, // :3
    BN_CTX *ctx); // :3

/** Gets the jacobian projective coordinates of a EC_POINT over GFp
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM for the x-coordinate
 *  \param  y      BIGNUM for the y-coordinate
 *  \param  z      BIGNUM for the z-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_get_Jprojective_coordinates_GFp(const EC_GROUP *group, const EC_POINT *p, // :3
    BIGNUM *x, BIGNUM *y, BIGNUM *z, BN_CTX *ctx); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Sets the affine coordinates of an EC_POINT
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with the x-coordinate
 *  \param  y      BIGNUM with the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_set_affine_coordinates(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, const BIGNUM *y, // :3
    BN_CTX *ctx); // :3

/** Gets the affine coordinates of an EC_POINT.
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM for the x-coordinate
 *  \param  y      BIGNUM for the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_get_affine_coordinates(const EC_GROUP *group, const EC_POINT *p, // :3
    BIGNUM *x, BIGNUM *y, BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Sets the affine coordinates of an EC_POINT. A synonym of
 *  EC_POINT_set_affine_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with the x-coordinate
 *  \param  y      BIGNUM with the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_set_affine_coordinates_GFp(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, const BIGNUM *y, BN_CTX *ctx); // :3

/** Gets the affine coordinates of an EC_POINT. A synonym of
 *  EC_POINT_get_affine_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM for the x-coordinate
 *  \param  y      BIGNUM for the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_get_affine_coordinates_GFp(const EC_GROUP *group, const EC_POINT *p, // :3
    BIGNUM *x, BIGNUM *y, BN_CTX *ctx); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Sets the x9.62 compressed coordinates of a EC_POINT
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with x-coordinate
 *  \param  y_bit  integer with the y-Bit (either 0 or 1)
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_set_compressed_coordinates(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, int y_bit, // :3
    BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Sets the x9.62 compressed coordinates of a EC_POINT. A synonym of
 *  EC_POINT_set_compressed_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with x-coordinate
 *  \param  y_bit  integer with the y-Bit (either 0 or 1)
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_set_compressed_coordinates_GFp(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, int y_bit, BN_CTX *ctx); // :3
#ifndef OPENSSL_NO_EC2M // :3
/** Sets the affine coordinates of an EC_POINT. A synonym of
 *  EC_POINT_set_affine_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with the x-coordinate
 *  \param  y      BIGNUM with the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_set_affine_coordinates_GF2m(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, const BIGNUM *y, BN_CTX *ctx); // :3

/** Gets the affine coordinates of an EC_POINT. A synonym of
 *  EC_POINT_get_affine_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM for the x-coordinate
 *  \param  y      BIGNUM for the y-coordinate
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_get_affine_coordinates_GF2m(const EC_GROUP *group, const EC_POINT *p, // :3
    BIGNUM *x, BIGNUM *y, BN_CTX *ctx); // :3

/** Sets the x9.62 compressed coordinates of a EC_POINT. A synonym of
 *  EC_POINT_set_compressed_coordinates
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  x      BIGNUM with x-coordinate
 *  \param  y_bit  integer with the y-Bit (either 0 or 1)
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_set_compressed_coordinates_GF2m(const EC_GROUP *group, EC_POINT *p, // :3
    const BIGNUM *x, int y_bit, BN_CTX *ctx); // :3
#endif // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Encodes a EC_POINT object to a octet string
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  form   point conversion form
 *  \param  buf    memory buffer for the result. If NULL the function returns
 *                 required buffer size.
 *  \param  len    length of the memory buffer
 *  \param  ctx    BN_CTX object (optional)
 *  \return the length of the encoded octet string or 0 if an error occurred
 */ // :3
size_t EC_POINT_point2oct(const EC_GROUP *group, const EC_POINT *p, // :3
    point_conversion_form_t form, // :3
    unsigned char *buf, size_t len, BN_CTX *ctx); // :3

/** Decodes a EC_POINT from a octet string
 *  \param  group  underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \param  buf    memory buffer with the encoded ec point
 *  \param  len    length of the encoded ec point
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_oct2point(const EC_GROUP *group, EC_POINT *p, // :3
    const unsigned char *buf, size_t len, BN_CTX *ctx); // :3

/** Encodes an EC_POINT object to an allocated octet string
 *  \param  group  underlying EC_GROUP object
 *  \param  point  EC_POINT object
 *  \param  form   point conversion form
 *  \param  pbuf   returns pointer to allocated buffer
 *  \param  ctx    BN_CTX object (optional)
 *  \return the length of the encoded octet string or 0 if an error occurred
 */ // :3
size_t EC_POINT_point2buf(const EC_GROUP *group, const EC_POINT *point, // :3
    point_conversion_form_t form, // :3
    unsigned char **pbuf, BN_CTX *ctx); // :3

/* other interfaces to point2oct/oct2point: */
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 BIGNUM *EC_POINT_point2bn(const EC_GROUP *,
    const EC_POINT *,
    point_conversion_form_t form,
    BIGNUM *, BN_CTX *);
OSSL_DEPRECATEDIN_3_0 EC_POINT *EC_POINT_bn2point(const EC_GROUP *,
    const BIGNUM *,
    EC_POINT *, BN_CTX *);
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

char *EC_POINT_point2hex(const EC_GROUP *, const EC_POINT *,
    point_conversion_form_t form, BN_CTX *);
EC_POINT *EC_POINT_hex2point(const EC_GROUP *, const char *,
    EC_POINT *, BN_CTX *);

/********************************************************************/
/*         functions for doing EC_POINT arithmetic                  */
/********************************************************************/

/** Computes the sum of two EC_POINT
 *  \param  group  underlying EC_GROUP object
 *  \param  r      EC_POINT object for the result (r = a + b)
 *  \param  a      EC_POINT object with the first summand
 *  \param  b      EC_POINT object with the second summand
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_add(const EC_GROUP *group, EC_POINT *r, const EC_POINT *a, // :3
    const EC_POINT *b, BN_CTX *ctx); // :3

/** Computes the double of a EC_POINT
 *  \param  group  underlying EC_GROUP object
 *  \param  r      EC_POINT object for the result (r = 2 * a)
 *  \param  a      EC_POINT object
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_dbl(const EC_GROUP *group, EC_POINT *r, const EC_POINT *a, // :3
    BN_CTX *ctx); // :3

/** Computes the inverse of a EC_POINT
 *  \param  group  underlying EC_GROUP object
 *  \param  a      EC_POINT object to be inverted (it's used for the result as well)
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_invert(const EC_GROUP *group, EC_POINT *a, BN_CTX *ctx); // :3

/** Checks whether the point is the neutral element of the group
 *  \param  group  the underlying EC_GROUP object
 *  \param  p      EC_POINT object
 *  \return 1 if the point is the neutral element and 0 otherwise
 */ // :3
int EC_POINT_is_at_infinity(const EC_GROUP *group, const EC_POINT *p); // :3

/** Checks whether the point is on the curve
 *  \param  group  underlying EC_GROUP object
 *  \param  point  EC_POINT object to check
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 if the point is on the curve, 0 if not, or -1 on error
 */ // :3
int EC_POINT_is_on_curve(const EC_GROUP *group, const EC_POINT *point, // :3
    BN_CTX *ctx); // :3

/** Compares two EC_POINTs
 *  \param  group  underlying EC_GROUP object
 *  \param  a      first EC_POINT object
 *  \param  b      second EC_POINT object
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 if the points are not equal, 0 if they are, or -1 on error
 */ // :3
int EC_POINT_cmp(const EC_GROUP *group, const EC_POINT *a, const EC_POINT *b, // :3
    BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINT_make_affine(const EC_GROUP *group, // :3
    EC_POINT *point, BN_CTX *ctx); // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINTs_make_affine(const EC_GROUP *group, size_t num, // :3
    EC_POINT *points[], BN_CTX *ctx); // :3

/** Computes r = generator * n + sum_{i=0}^{num-1} p[i] * m[i]
 *  \param  group  underlying EC_GROUP object
 *  \param  r      EC_POINT object for the result
 *  \param  n      BIGNUM with the multiplier for the group generator (optional)
 *  \param  num    number further summands
 *  \param  p      array of size num of EC_POINT objects
 *  \param  m      array of size num of BIGNUM objects
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_POINTs_mul(const EC_GROUP *group, EC_POINT *r, // :3
    const BIGNUM *n, size_t num, // :3
    const EC_POINT *p[], const BIGNUM *m[], // :3
    BN_CTX *ctx); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/** Computes r = generator * n + q * m
 *  \param  group  underlying EC_GROUP object
 *  \param  r      EC_POINT object for the result
 *  \param  n      BIGNUM with the multiplier for the group generator (optional)
 *  \param  q      EC_POINT object with the first factor of the second summand
 *  \param  m      BIGNUM with the second factor of the second summand
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
int EC_POINT_mul(const EC_GROUP *group, EC_POINT *r, const BIGNUM *n, // :3
    const EC_POINT *q, const BIGNUM *m, BN_CTX *ctx); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Stores multiples of generator for faster point multiplication
 *  \param  group  EC_GROUP object
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_precompute_mult(EC_GROUP *group, BN_CTX *ctx); // :3

/** Reports whether a precomputation has been done
 *  \param  group  EC_GROUP object
 *  \return 1 if a pre-computation has been done and 0 otherwise
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_GROUP_have_precompute_mult(const EC_GROUP *group); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/********************************************************************/
/*                       ASN1 stuff                                 */
/********************************************************************/

DECLARE_ASN1_ITEM(ECPKPARAMETERS)
DECLARE_ASN1_ALLOC_FUNCTIONS(ECPKPARAMETERS)
DECLARE_ASN1_ITEM(ECPARAMETERS)
DECLARE_ASN1_ALLOC_FUNCTIONS(ECPARAMETERS)

/*
 * EC_GROUP_get_basis_type() returns the NID of the basis type used to
 * represent the field elements
 */ // :3
int EC_GROUP_get_basis_type(const EC_GROUP *); // :3
#ifndef OPENSSL_NO_EC2M // :3
int EC_GROUP_get_trinomial_basis(const EC_GROUP *, unsigned int *k); // :3
int EC_GROUP_get_pentanomial_basis(const EC_GROUP *, unsigned int *k1, // :3
    unsigned int *k2, unsigned int *k3); // :3
#endif // :3

EC_GROUP *d2i_ECPKParameters(EC_GROUP **, const unsigned char **in, long len); // :3
int i2d_ECPKParameters(const EC_GROUP *, unsigned char **out); // :3

#define d2i_ECPKParameters_bio(bp, x) \
    ASN1_d2i_bio_of(EC_GROUP, NULL, d2i_ECPKParameters, bp, x) // :3
#define i2d_ECPKParameters_bio(bp, x) \
    ASN1_i2d_bio_of(EC_GROUP, i2d_ECPKParameters, bp, x) // :3
#define d2i_ECPKParameters_fp(fp, x)                                       \
    (EC_GROUP *)ASN1_d2i_fp(NULL, (d2i_of_void *)d2i_ECPKParameters, (fp), \
        (void **)(x)) // :3
#define i2d_ECPKParameters_fp(fp, x) \
    ASN1_i2d_fp((i2d_of_void *)i2d_ECPKParameters, (fp), (void *)(x)) // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 int ECPKParameters_print(BIO *bp, const EC_GROUP *x, // :3
    int off); // :3
#ifndef OPENSSL_NO_STDIO // :3
OSSL_DEPRECATEDIN_3_0 int ECPKParameters_print_fp(FILE *fp, const EC_GROUP *x, // :3
    int off); // :3
#endif // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

/********************************************************************/
/*                      EC_KEY functions                            */
/********************************************************************/

/* some values for the encoding_flag */
#define EC_PKEY_NO_PARAMETERS 0x001
#define EC_PKEY_NO_PUBKEY 0x002

/* some values for the flags field */
#define EC_FLAG_SM2_RANGE 0x0004
#define EC_FLAG_COFACTOR_ECDH 0x1000
#define EC_FLAG_CHECK_NAMED_GROUP 0x2000
#define EC_FLAG_CHECK_NAMED_GROUP_NIST 0x4000
#define EC_FLAG_CHECK_NAMED_GROUP_MASK \
    (EC_FLAG_CHECK_NAMED_GROUP | EC_FLAG_CHECK_NAMED_GROUP_NIST)

/* Deprecated flags -  it was using 0x01..0x02 */
#define EC_FLAG_NON_FIPS_ALLOW 0x0000
#define EC_FLAG_FIPS_CHECKED 0x0000

#ifndef OPENSSL_NO_DEPRECATED_3_0
/**
 *  Creates a new EC_KEY object.
 *  \param  ctx  The library context for to use for this EC_KEY. May be NULL in
 *               which case the default library context is used.
 *  \return EC_KEY object or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_new_ex(OSSL_LIB_CTX *ctx, const char *propq); // :3

/**
 *  Creates a new EC_KEY object. Same as calling EC_KEY_new_ex with a
 *  NULL library context
 *  \return EC_KEY object or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_new(void); // :3

OSSL_DEPRECATEDIN_3_0 int EC_KEY_get_flags(const EC_KEY *key); // :3

OSSL_DEPRECATEDIN_3_0 void EC_KEY_set_flags(EC_KEY *key, int flags); // :3

OSSL_DEPRECATEDIN_3_0 void EC_KEY_clear_flags(EC_KEY *key, int flags); // :3

OSSL_DEPRECATEDIN_3_0 int EC_KEY_decoded_from_explicit_params(const EC_KEY *key); // :3

/**
 *  Creates a new EC_KEY object using a named curve as underlying
 *  EC_GROUP object.
 *  \param  ctx   The library context for to use for this EC_KEY. May be NULL in
 *                which case the default library context is used.
 *  \param  propq Any property query string
 *  \param  nid   NID of the named curve.
 *  \return EC_KEY object or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_new_by_curve_name_ex(OSSL_LIB_CTX *ctx, // :3
    const char *propq, // :3
    int nid); // :3

/**
 *  Creates a new EC_KEY object using a named curve as underlying
 *  EC_GROUP object. Same as calling EC_KEY_new_by_curve_name_ex with a NULL
 *  library context and property query string.
 *  \param  nid  NID of the named curve.
 *  \return EC_KEY object or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_new_by_curve_name(int nid); // :3

/** Frees a EC_KEY object.
 *  \param  key  EC_KEY object to be freed.
 */ // :3
OSSL_DEPRECATEDIN_3_0 void EC_KEY_free(EC_KEY *key); // :3

/** Copies a EC_KEY object.
 *  \param  dst  destination EC_KEY object
 *  \param  src  src EC_KEY object
 *  \return dst or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_copy(EC_KEY *dst, const EC_KEY *src); // :3

/** Creates a new EC_KEY object and copies the content from src to it.
 *  \param  src  the source EC_KEY object
 *  \return newly created EC_KEY object or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_dup(const EC_KEY *src); // :3

/** Increases the internal reference count of a EC_KEY object.
 *  \param  key  EC_KEY object
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_up_ref(EC_KEY *key); // :3

/** Returns the ENGINE object of a EC_KEY object
 *  \param  eckey  EC_KEY object
 *  \return the ENGINE object (possibly NULL).
 */ // :3
OSSL_DEPRECATEDIN_3_0 ENGINE *EC_KEY_get0_engine(const EC_KEY *eckey); // :3

/** Returns the EC_GROUP object of a EC_KEY object
 *  \param  key  EC_KEY object
 *  \return the EC_GROUP object (possibly NULL).
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_GROUP *EC_KEY_get0_group(const EC_KEY *key); // :3

/** Sets the EC_GROUP of a EC_KEY object.
 *  \param  key    EC_KEY object
 *  \param  group  EC_GROUP to use in the EC_KEY object (note: the EC_KEY
 *                 object will use an own copy of the EC_GROUP).
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_group(EC_KEY *key, const EC_GROUP *group); // :3

/** Returns the private key of a EC_KEY object.
 *  \param  key  EC_KEY object
 *  \return a BIGNUM with the private key (possibly NULL).
 */ // :3
OSSL_DEPRECATEDIN_3_0 const BIGNUM *EC_KEY_get0_private_key(const EC_KEY *key); // :3

/** Sets the private key of a EC_KEY object.
 *  \param  key  EC_KEY object
 *  \param  prv  BIGNUM with the private key (note: the EC_KEY object
 *               will use an own copy of the BIGNUM).
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_private_key(EC_KEY *key, const BIGNUM *prv); // :3

/** Returns the public key of a EC_KEY object.
 *  \param  key  the EC_KEY object
 *  \return a EC_POINT object with the public key (possibly NULL)
 */ // :3
OSSL_DEPRECATEDIN_3_0 const EC_POINT *EC_KEY_get0_public_key(const EC_KEY *key); // :3

/** Sets the public key of a EC_KEY object.
 *  \param  key  EC_KEY object
 *  \param  pub  EC_POINT object with the public key (note: the EC_KEY object
 *               will use an own copy of the EC_POINT object).
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_public_key(EC_KEY *key, const EC_POINT *pub); // :3

OSSL_DEPRECATEDIN_3_0 unsigned EC_KEY_get_enc_flags(const EC_KEY *key); // :3
OSSL_DEPRECATEDIN_3_0 void EC_KEY_set_enc_flags(EC_KEY *eckey, unsigned int flags); // :3
OSSL_DEPRECATEDIN_3_0 point_conversion_form_t EC_KEY_get_conv_form(const EC_KEY *key); // :3
OSSL_DEPRECATEDIN_3_0 void EC_KEY_set_conv_form(EC_KEY *eckey, // :3
    point_conversion_form_t cform); // :3
#endif /*OPENSSL_NO_DEPRECATED_3_0 */

#define EC_KEY_get_ex_new_index(l, p, newf, dupf, freef) \
    CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_EC_KEY, l, p, newf, dupf, freef)

#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_ex_data(EC_KEY *key, int idx, void *arg);
OSSL_DEPRECATEDIN_3_0 void *EC_KEY_get_ex_data(const EC_KEY *key, int idx);

/* wrapper functions for the underlying EC_GROUP object */
OSSL_DEPRECATEDIN_3_0 void EC_KEY_set_asn1_flag(EC_KEY *eckey, int asn1_flag);

/** Creates a table of pre-computed multiples of the generator to
 *  accelerate further EC_KEY operations.
 *  \param  key  EC_KEY object
 *  \param  ctx  BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_precompute_mult(EC_KEY *key, BN_CTX *ctx); // :3

/** Creates a new ec private (and optional a new public) key.
 *  \param  key  EC_KEY object
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_generate_key(EC_KEY *key); // :3

/** Verifies that a private and/or public key is valid.
 *  \param  key  the EC_KEY object
 *  \return 1 on success and 0 otherwise.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_check_key(const EC_KEY *key); // :3

/** Indicates if an EC_KEY can be used for signing.
 *  \param  eckey  the EC_KEY object
 *  \return 1 if can sign and 0 otherwise.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_can_sign(const EC_KEY *eckey); // :3

/** Sets a public key from affine coordinates performing
 *  necessary NIST PKV tests.
 *  \param  key  the EC_KEY object
 *  \param  x    public key x coordinate
 *  \param  y    public key y coordinate
 *  \return 1 on success and 0 otherwise.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_public_key_affine_coordinates(EC_KEY *key, // :3
    BIGNUM *x, // :3
    BIGNUM *y); // :3

/** Encodes an EC_KEY public key to an allocated octet string
 *  \param  key    key to encode
 *  \param  form   point conversion form
 *  \param  pbuf   returns pointer to allocated buffer
 *  \param  ctx    BN_CTX object (optional)
 *  \return the length of the encoded octet string or 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 size_t EC_KEY_key2buf(const EC_KEY *key, // :3
    point_conversion_form_t form, // :3
    unsigned char **pbuf, BN_CTX *ctx); // :3

/** Decodes a EC_KEY public key from a octet string
 *  \param  key    key to decode
 *  \param  buf    memory buffer with the encoded ec point
 *  \param  len    length of the encoded ec point
 *  \param  ctx    BN_CTX object (optional)
 *  \return 1 on success and 0 if an error occurred
 */ // :3

OSSL_DEPRECATEDIN_3_0 int EC_KEY_oct2key(EC_KEY *key, const unsigned char *buf, // :3
    size_t len, BN_CTX *ctx); // :3

/** Decodes an EC_KEY private key from an octet string
 *  \param  key    key to decode
 *  \param  buf    memory buffer with the encoded private key
 *  \param  len    length of the encoded key
 *  \return 1 on success and 0 if an error occurred
 */ // :3

OSSL_DEPRECATEDIN_3_0 int EC_KEY_oct2priv(EC_KEY *key, const unsigned char *buf, // :3
    size_t len); // :3

/** Encodes a EC_KEY private key to an octet string
 *  \param  key    key to encode
 *  \param  buf    memory buffer for the result. If NULL the function returns
 *                 required buffer size.
 *  \param  len    length of the memory buffer
 *  \return the length of the encoded octet string or 0 if an error occurred
 */ // :3

OSSL_DEPRECATEDIN_3_0 size_t EC_KEY_priv2oct(const EC_KEY *key, // :3
    unsigned char *buf, size_t len); // :3

/** Encodes an EC_KEY private key to an allocated octet string
 *  \param  eckey  key to encode
 *  \param  pbuf   returns pointer to allocated buffer
 *  \return the length of the encoded octet string or 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 size_t EC_KEY_priv2buf(const EC_KEY *eckey, // :3
    unsigned char **pbuf); // :3

/********************************************************************/
/*        de- and encoding functions for SEC1 ECPrivateKey          */
/********************************************************************/

/** Decodes a private key from a memory buffer.
 *  \param  key  a pointer to a EC_KEY object which should be used (or NULL)
 *  \param  in   pointer to memory with the DER encoded private key
 *  \param  len  length of the DER encoded private key
 *  \return the decoded private key or NULL if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *d2i_ECPrivateKey(EC_KEY **key, // :3
    const unsigned char **in, // :3
    long len); // :3

/** Encodes a private key object and stores the result in a buffer.
 *  \param  key  the EC_KEY object to encode
 *  \param  out  the buffer for the result (if NULL the function returns number
 *               of bytes needed).
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int i2d_ECPrivateKey(const EC_KEY *key, // :3
    unsigned char **out); // :3

/********************************************************************/
/*        de- and encoding functions for EC parameters              */
/********************************************************************/

/** Decodes ec parameter from a memory buffer.
 *  \param  key  a pointer to a EC_KEY object which should be used (or NULL)
 *  \param  in   pointer to memory with the DER encoded ec parameters
 *  \param  len  length of the DER encoded ec parameters
 *  \return a EC_KEY object with the decoded parameters or NULL if an error
 *          occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *d2i_ECParameters(EC_KEY **key, // :3
    const unsigned char **in, // :3
    long len); // :3

/** Encodes ec parameter and stores the result in a buffer.
 *  \param  key  the EC_KEY object with ec parameters to encode
 *  \param  out  the buffer for the result (if NULL the function returns number
 *               of bytes needed).
 *  \return 1 on success and 0 if an error occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 int i2d_ECParameters(const EC_KEY *key, // :3
    unsigned char **out); // :3

/********************************************************************/
/*         de- and encoding functions for EC public key             */
/*         (octet string, not DER -- hence 'o2i' and 'i2o')         */
/********************************************************************/

/** Decodes an ec public key from a octet string.
 *  \param  key  a pointer to a EC_KEY object which should be used
 *  \param  in   memory buffer with the encoded public key
 *  \param  len  length of the encoded public key
 *  \return EC_KEY object with decoded public key or NULL if an error
 *          occurred.
 */ // :3
OSSL_DEPRECATEDIN_3_0 EC_KEY *o2i_ECPublicKey(EC_KEY **key, // :3
    const unsigned char **in, long len); // :3

/** Encodes an ec public key in an octet string.
 *  \param  key  the EC_KEY object with the public key
 *  \param  out  the buffer for the result (if NULL the function returns number
 *               of bytes needed).
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int i2o_ECPublicKey(const EC_KEY *key, unsigned char **out); // :3

/** Prints out the ec parameters on human readable form.
 *  \param  bp   BIO object to which the information is printed
 *  \param  key  EC_KEY object
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECParameters_print(BIO *bp, const EC_KEY *key); // :3

/** Prints out the contents of a EC_KEY object
 *  \param  bp   BIO object to which the information is printed
 *  \param  key  EC_KEY object
 *  \param  off  line offset
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_print(BIO *bp, const EC_KEY *key, int off); // :3

#ifndef OPENSSL_NO_STDIO // :3
/** Prints out the ec parameters on human readable form.
 *  \param  fp   file descriptor to which the information is printed
 *  \param  key  EC_KEY object
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECParameters_print_fp(FILE *fp, const EC_KEY *key); // :3

/** Prints out the contents of a EC_KEY object
 *  \param  fp   file descriptor to which the information is printed
 *  \param  key  EC_KEY object
 *  \param  off  line offset
 *  \return 1 on success and 0 if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 int EC_KEY_print_fp(FILE *fp, const EC_KEY *key, int off); // :3
#endif /* OPENSSL_NO_STDIO */

OSSL_DEPRECATEDIN_3_0 const EC_KEY_METHOD *EC_KEY_OpenSSL(void);
OSSL_DEPRECATEDIN_3_0 const EC_KEY_METHOD *EC_KEY_get_default_method(void);
OSSL_DEPRECATEDIN_3_0 void EC_KEY_set_default_method(const EC_KEY_METHOD *meth);
OSSL_DEPRECATEDIN_3_0 const EC_KEY_METHOD *EC_KEY_get_method(const EC_KEY *key);
OSSL_DEPRECATEDIN_3_0 int EC_KEY_set_method(EC_KEY *key, const EC_KEY_METHOD *meth);
OSSL_DEPRECATEDIN_3_0 EC_KEY *EC_KEY_new_method(ENGINE *engine);

/** The old name for ecdh_KDF_X9_63
 *  The ECDH KDF specification has been mistakenly attributed to ANSI X9.62,
 *  it is actually specified in ANSI X9.63.
 *  This identifier is retained for backwards compatibility
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDH_KDF_X9_62(unsigned char *out, size_t outlen, // :3
    const unsigned char *Z, size_t Zlen, // :3
    const unsigned char *sinfo, // :3
    size_t sinfolen, const EVP_MD *md); // :3

OSSL_DEPRECATEDIN_3_0 int ECDH_compute_key(void *out, size_t outlen, // :3
    const EC_POINT *pub_key, // :3
    const EC_KEY *ecdh, // :3
    void *(*KDF)(const void *in, // :3
        size_t inlen, void *out, // :3
        size_t *outlen)); // :3
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

typedef struct ECDSA_SIG_st ECDSA_SIG;

/** Allocates and initialize a ECDSA_SIG structure
 *  \return pointer to a ECDSA_SIG structure or NULL if an error occurred
 */ // :3
ECDSA_SIG *ECDSA_SIG_new(void); // :3

/** frees a ECDSA_SIG structure
 *  \param  sig  pointer to the ECDSA_SIG structure
 */ // :3
void ECDSA_SIG_free(ECDSA_SIG *sig); // :3

/** i2d_ECDSA_SIG encodes content of ECDSA_SIG (note: this function modifies *pp
 *  (*pp += length of the DER encoded signature)).
 *  \param  sig  pointer to the ECDSA_SIG object
 *  \param  pp   pointer to a unsigned char pointer for the output or NULL
 *  \return the length of the DER encoded ECDSA_SIG object or a negative value
 *          on error
 */ // :3
DECLARE_ASN1_ENCODE_FUNCTIONS_only(ECDSA_SIG, ECDSA_SIG) // :3

/** d2i_ECDSA_SIG decodes an ECDSA signature (note: this function modifies *pp
 *  (*pp += len)).
 *  \param  sig  pointer to ECDSA_SIG pointer (may be NULL)
 *  \param  pp   memory buffer with the DER encoded signature
 *  \param  len  length of the buffer
 *  \return pointer to the decoded ECDSA_SIG structure (or NULL)
 */ // :3

/** Accessor for r and s fields of ECDSA_SIG
 *  \param  sig  pointer to ECDSA_SIG structure
 *  \param  pr   pointer to BIGNUM pointer for r (may be NULL)
 *  \param  ps   pointer to BIGNUM pointer for s (may be NULL)
 */ // :3
void ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps); // :3

/** Accessor for r field of ECDSA_SIG
 *  \param  sig  pointer to ECDSA_SIG structure
 */ // :3
const BIGNUM *ECDSA_SIG_get0_r(const ECDSA_SIG *sig); // :3

/** Accessor for s field of ECDSA_SIG
 *  \param  sig  pointer to ECDSA_SIG structure
 */ // :3
const BIGNUM *ECDSA_SIG_get0_s(const ECDSA_SIG *sig); // :3

/** Setter for r and s fields of ECDSA_SIG
 *  \param  sig  pointer to ECDSA_SIG structure
 *  \param  r    pointer to BIGNUM for r
 *  \param  s    pointer to BIGNUM for s
 */ // :3
int ECDSA_SIG_set0(ECDSA_SIG *sig, BIGNUM *r, BIGNUM *s); // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
/** Computes the ECDSA signature of the given hash value using
 *  the supplied private key and returns the created signature.
 *  \param  dgst      pointer to the hash value
 *  \param  dgst_len  length of the hash value
 *  \param  eckey     EC_KEY object containing a private EC key
 *  \return pointer to a ECDSA_SIG structure or NULL if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 ECDSA_SIG *ECDSA_do_sign(const unsigned char *dgst, // :3
    int dgst_len, EC_KEY *eckey); // :3

/** Computes ECDSA signature of a given hash value using the supplied
 *  private key (note: sig must point to ECDSA_size(eckey) bytes of memory).
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  kinv     BIGNUM with a pre-computed inverse k (optional)
 *  \param  rp       BIGNUM with a pre-computed rp value (optional),
 *                   see ECDSA_sign_setup
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return pointer to a ECDSA_SIG structure or NULL if an error occurred
 */ // :3
OSSL_DEPRECATEDIN_3_0 ECDSA_SIG *ECDSA_do_sign_ex(const unsigned char *dgst, // :3
    int dgstlen, const BIGNUM *kinv, // :3
    const BIGNUM *rp, EC_KEY *eckey); // :3

/** Verifies that the supplied signature is a valid ECDSA
 *  signature of the supplied hash value using the supplied public key.
 *  \param  dgst      pointer to the hash value
 *  \param  dgst_len  length of the hash value
 *  \param  sig       ECDSA_SIG structure
 *  \param  eckey     EC_KEY object containing a public EC key
 *  \return 1 if the signature is valid, 0 if the signature is invalid
 *          and -1 on error
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_do_verify(const unsigned char *dgst, int dgst_len, // :3
    const ECDSA_SIG *sig, EC_KEY *eckey); // :3

/** Precompute parts of the signing operation
 *  \param  eckey  EC_KEY object containing a private EC key
 *  \param  ctx    BN_CTX object (optional)
 *  \param  kinv   BIGNUM pointer for the inverse of k
 *  \param  rp     BIGNUM pointer for x coordinate of k * generator
 *  \return 1 on success and 0 otherwise
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_sign_setup(EC_KEY *eckey, BN_CTX *ctx, // :3
    BIGNUM **kinv, BIGNUM **rp); // :3

/** Computes ECDSA signature of a given hash value using the supplied
 *  private key (note: sig must point to ECDSA_size(eckey) bytes of memory).
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  sig      memory for the DER encoded created signature
 *  \param  siglen   pointer to the length of the returned signature
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return 1 on success and 0 otherwise
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_sign(int type, const unsigned char *dgst, // :3
    int dgstlen, unsigned char *sig, // :3
    unsigned int *siglen, EC_KEY *eckey); // :3

/** Computes ECDSA signature of a given hash value using the supplied
 *  private key (note: sig must point to ECDSA_size(eckey) bytes of memory).
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  sig      buffer to hold the DER encoded signature
 *  \param  siglen   pointer to the length of the returned signature
 *  \param  kinv     BIGNUM with a pre-computed inverse k (optional)
 *  \param  rp       BIGNUM with a pre-computed rp value (optional),
 *                   see ECDSA_sign_setup
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return 1 on success and 0 otherwise
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_sign_ex(int type, const unsigned char *dgst, // :3
    int dgstlen, unsigned char *sig, // :3
    unsigned int *siglen, const BIGNUM *kinv, // :3
    const BIGNUM *rp, EC_KEY *eckey); // :3

/** Verifies that the given signature is valid ECDSA signature
 *  of the supplied hash value using the specified public key.
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value
 *  \param  dgstlen  length of the hash value
 *  \param  sig      pointer to the DER encoded signature
 *  \param  siglen   length of the DER encoded signature
 *  \param  eckey    EC_KEY object containing a public EC key
 *  \return 1 if the signature is valid, 0 if the signature is invalid
 *          and -1 on error
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_verify(int type, const unsigned char *dgst, // :3
    int dgstlen, const unsigned char *sig, // :3
    int siglen, EC_KEY *eckey); // :3

/** Returns the maximum length of the DER encoded signature
 *  \param  eckey  EC_KEY object
 *  \return numbers of bytes required for the DER encoded signature
 */ // :3
OSSL_DEPRECATEDIN_3_0 int ECDSA_size(const EC_KEY *eckey); // :3

/********************************************************************/
/*  EC_KEY_METHOD constructors, destructors, writers and accessors  */
/********************************************************************/

OSSL_DEPRECATEDIN_3_0 EC_KEY_METHOD *EC_KEY_METHOD_new(const EC_KEY_METHOD *meth);
OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_free(EC_KEY_METHOD *meth);
OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_set_init(EC_KEY_METHOD *meth,
    int (*init)(EC_KEY *key),
    void (*finish)(EC_KEY *key),
    int (*copy)(EC_KEY *dest, const EC_KEY *src),
    int (*set_group)(EC_KEY *key, const EC_GROUP *grp),
    int (*set_private)(EC_KEY *key, const BIGNUM *priv_key),
    int (*set_public)(EC_KEY *key, const EC_POINT *pub_key));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_set_keygen(EC_KEY_METHOD *meth,
    int (*keygen)(EC_KEY *key));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_set_compute_key(EC_KEY_METHOD *meth,
    int (*ckey)(unsigned char **psec, size_t *pseclen,
        const EC_POINT *pub_key, const EC_KEY *ecdh));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_set_sign(EC_KEY_METHOD *meth,
    int (*sign)(int type, const unsigned char *dgst,
        int dlen, unsigned char *sig,
        unsigned int *siglen,
        const BIGNUM *kinv, const BIGNUM *r,
        EC_KEY *eckey),
    int (*sign_setup)(EC_KEY *eckey, BN_CTX *ctx_in,
        BIGNUM **kinvp, BIGNUM **rp),
    ECDSA_SIG *(*sign_sig)(const unsigned char *dgst,
        int dgst_len,
        const BIGNUM *in_kinv,
        const BIGNUM *in_r,
        EC_KEY *eckey));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_set_verify(EC_KEY_METHOD *meth,
    int (*verify)(int type, const unsigned char *dgst, int dgst_len,
        const unsigned char *sigbuf,
        int sig_len, EC_KEY *eckey),
    int (*verify_sig)(const unsigned char *dgst,
        int dgst_len, const ECDSA_SIG *sig,
        EC_KEY *eckey));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_get_init(const EC_KEY_METHOD *meth,
    int (**pinit)(EC_KEY *key),
    void (**pfinish)(EC_KEY *key),
    int (**pcopy)(EC_KEY *dest, const EC_KEY *src),
    int (**pset_group)(EC_KEY *key, const EC_GROUP *grp),
    int (**pset_private)(EC_KEY *key, const BIGNUM *priv_key),
    int (**pset_public)(EC_KEY *key, const EC_POINT *pub_key));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_get_keygen(const EC_KEY_METHOD *meth, int (**pkeygen)(EC_KEY *key));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_get_compute_key(const EC_KEY_METHOD *meth,
    int (**pck)(unsigned char **psec,
        size_t *pseclen,
        const EC_POINT *pub_key,
        const EC_KEY *ecdh));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_get_sign(const EC_KEY_METHOD *meth,
    int (**psign)(int type, const unsigned char *dgst,
        int dlen, unsigned char *sig,
        unsigned int *siglen,
        const BIGNUM *kinv, const BIGNUM *r,
        EC_KEY *eckey),
    int (**psign_setup)(EC_KEY *eckey, BN_CTX *ctx_in,
        BIGNUM **kinvp, BIGNUM **rp),
    ECDSA_SIG *(**psign_sig)(const unsigned char *dgst,
        int dgst_len,
        const BIGNUM *in_kinv,
        const BIGNUM *in_r,
        EC_KEY *eckey));

OSSL_DEPRECATEDIN_3_0 void EC_KEY_METHOD_get_verify(const EC_KEY_METHOD *meth,
    int (**pverify)(int type, const unsigned char *dgst, int dgst_len,
        const unsigned char *sigbuf,
        int sig_len, EC_KEY *eckey),
    int (**pverify_sig)(const unsigned char *dgst,
        int dgst_len,
        const ECDSA_SIG *sig,
        EC_KEY *eckey));
#endif /* OPENSSL_NO_DEPRECATED_3_0 */

#define EVP_EC_gen(curve) \
    EVP_PKEY_Q_keygen(NULL, NULL, "EC", (char *)(strstr(curve, "")))
/* strstr is used to enable type checking for the variadic string arg */
#define ECParameters_dup(x) ASN1_dup_of(EC_KEY, i2d_ECParameters, \
    d2i_ECParameters, x)

#ifndef __cplusplus
#if defined(__SUNPRO_C)
#if __SUNPRO_C >= 0x520
#pragma error_messages(default, E_ARRAY_OF_INCOMPLETE_NONAME, E_ARRAY_OF_INCOMPLETE)
#endif
#endif
#endif

#endif
#ifdef __cplusplus
}
#endif
#endif
