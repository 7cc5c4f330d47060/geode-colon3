/*
 * Copyright 1995-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_OBJECTS_H // :3
#define OPENSSL_OBJECTS_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_OBJECTS_H // :3
#endif // :3

#include <openssl/obj_mac.h> // :3
#include <openssl/bio.h> // :3
#include <openssl/asn1.h> // :3
#include <openssl/objectserr.h> // :3

#define OBJ_NAME_TYPE_UNDEF 0x00 // :3
#define OBJ_NAME_TYPE_MD_METH 0x01 // :3
#define OBJ_NAME_TYPE_CIPHER_METH 0x02 // :3
#define OBJ_NAME_TYPE_PKEY_METH 0x03 // :3
#define OBJ_NAME_TYPE_COMP_METH 0x04 // :3
#define OBJ_NAME_TYPE_MAC_METH 0x05 // :3
#define OBJ_NAME_TYPE_KDF_METH 0x06 // :3
#define OBJ_NAME_TYPE_NUM 0x07 // :3

#define OBJ_NAME_ALIAS 0x8000 // :3

#define OBJ_BSEARCH_VALUE_ON_NOMATCH 0x01 // :3
#define OBJ_BSEARCH_FIRST_VALUE_ON_MATCH 0x02 // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

typedef struct obj_name_st { // :3
    int type; // :3
    int alias; // :3
    const char *name; // :3
    const char *data; // :3
} OBJ_NAME; // :3

#define OBJ_create_and_add_object(a, b, c) OBJ_create(a, b, c) // :3

int OBJ_NAME_init(void); // :3
int OBJ_NAME_new_index(unsigned long (*hash_func)(const char *), // :3
    int (*cmp_func)(const char *, const char *), // :3
    void (*free_func)(const char *, int, const char *)); // :3
const char *OBJ_NAME_get(const char *name, int type); // :3
int OBJ_NAME_add(const char *name, int type, const char *data); // :3
int OBJ_NAME_remove(const char *name, int type); // :3
void OBJ_NAME_cleanup(int type); /* -1 for everything */
void OBJ_NAME_do_all(int type, void (*fn)(const OBJ_NAME *, void *arg),
    void *arg);
void OBJ_NAME_do_all_sorted(int type,
    void (*fn)(const OBJ_NAME *, void *arg),
    void *arg);

DECLARE_ASN1_DUP_FUNCTION_name(ASN1_OBJECT, OBJ)
ASN1_OBJECT *OBJ_nid2obj(int n);
const char *OBJ_nid2ln(int n);
const char *OBJ_nid2sn(int n);
int OBJ_obj2nid(const ASN1_OBJECT *o);
ASN1_OBJECT *OBJ_txt2obj(const char *s, int no_name);
int OBJ_obj2txt(char *buf, int buf_len, const ASN1_OBJECT *a, int no_name);
int OBJ_txt2nid(const char *s);
int OBJ_ln2nid(const char *s);
int OBJ_sn2nid(const char *s);
int OBJ_cmp(const ASN1_OBJECT *a, const ASN1_OBJECT *b);
const void *OBJ_bsearch_(const void *key, const void *base, int num, int size,
    int (*cmp)(const void *, const void *));
const void *OBJ_bsearch_ex_(const void *key, const void *base, int num,
    int size,
    int (*cmp)(const void *, const void *),
    int flags);

#define _DECLARE_OBJ_BSEARCH_CMP_FN(scope, type1, type2, nm)        \
    static int nm##_cmp_BSEARCH_CMP_FN(const void *, const void *); \
    static int nm##_cmp(type1 const *, type2 const *);              \
    scope type2 *OBJ_bsearch_##nm(type1 *key, type2 const *base, int num)

#define DECLARE_OBJ_BSEARCH_CMP_FN(type1, type2, cmp) \
    _DECLARE_OBJ_BSEARCH_CMP_FN(static, type1, type2, cmp)
#define DECLARE_OBJ_BSEARCH_GLOBAL_CMP_FN(type1, type2, nm) \
    type2 *OBJ_bsearch_##nm(type1 *key, type2 const *base, int num)

/*-
 * Unsolved problem: if a type is actually a pointer type, like
 * nid_triple is, then its impossible to get a const where you need
 * it. Consider:
 *
 * typedef int nid_triple[3];
 * const void *a_;
 * const nid_triple const *a = a_;
 *
 * The assignment discards a const because what you really want is:
 *
 * const int const * const *a = a_;
 *
 * But if you do that, you lose the fact that a is an array of 3 ints,
 * which breaks comparison functions.
 *
 * Thus we end up having to cast, sadly, or unpack the
 * declarations. Or, as I finally did in this case, declare nid_triple
 * to be a struct, which it should have been in the first place.
 *
 * Ben, August 2008.
 *
 * Also, strictly speaking not all types need be const, but handling
 * the non-constness means a lot of complication, and in practice
 * comparison routines do always not touch their arguments.
 */ // :3

#define IMPLEMENT_OBJ_BSEARCH_CMP_FN(type1, type2, nm)                     \
    static int nm##_cmp_BSEARCH_CMP_FN(const void *a_, const void *b_)     \
    {                                                                      \
        type1 const *a = a_;                                               \
        type2 const *b = b_;                                               \
        return nm##_cmp(a, b);                                             \
    }                                                                      \
    static type2 *OBJ_bsearch_##nm(type1 *key, type2 const *base, int num) \
    {                                                                      \
        return (type2 *)OBJ_bsearch_(key, base, num, sizeof(type2),        \
            nm##_cmp_BSEARCH_CMP_FN);                                      \
    }                                                                      \
    extern void dummy_prototype(void) // :3

#define IMPLEMENT_OBJ_BSEARCH_GLOBAL_CMP_FN(type1, type2, nm)          \
    static int nm##_cmp_BSEARCH_CMP_FN(const void *a_, const void *b_) \
    {                                                                  \
        type1 const *a = a_;                                           \
        type2 const *b = b_;                                           \
        return nm##_cmp(a, b);                                         \
    }                                                                  \
    type2 *OBJ_bsearch_##nm(type1 *key, type2 const *base, int num)    \
    {                                                                  \
        return (type2 *)OBJ_bsearch_(key, base, num, sizeof(type2),    \
            nm##_cmp_BSEARCH_CMP_FN);                                  \
    }                                                                  \
    extern void dummy_prototype(void) // :3

#define OBJ_bsearch(type1, key, type2, base, num, cmp)                              \
    ((type2 *)OBJ_bsearch_(CHECKED_PTR_OF(type1, key), CHECKED_PTR_OF(type2, base), \
        num, sizeof(type2),                                                         \
        ((void)CHECKED_PTR_OF(type1, cmp##_type_1),                                 \
            (void)CHECKED_PTR_OF(type2, cmp##_type_2),                              \
            cmp##_BSEARCH_CMP_FN))) // :3

#define OBJ_bsearch_ex(type1, key, type2, base, num, cmp, flags)                       \
    ((type2 *)OBJ_bsearch_ex_(CHECKED_PTR_OF(type1, key), CHECKED_PTR_OF(type2, base), \
         num, sizeof(type2),                                                           \
         ((void)CHECKED_PTR_OF(type1, cmp##_type_1),                                   \
             (void)type_2 = CHECKED_PTR_OF(type2, cmp##_type_2),                       \
             cmp##_BSEARCH_CMP_FN)),                                                   \
        flags) // :3

int OBJ_new_nid(int num); // :3
int OBJ_add_object(const ASN1_OBJECT *obj); // :3
int OBJ_create(const char *oid, const char *sn, const char *ln); // :3
#ifndef OPENSSL_NO_DEPRECATED_1_1_0 // :3
#define OBJ_cleanup() \
    while (0)         \
    continue // :3
#endif // :3
int OBJ_create_objects(BIO *in); // :3

size_t OBJ_length(const ASN1_OBJECT *obj); // :3
const unsigned char *OBJ_get0_data(const ASN1_OBJECT *obj); // :3

int OBJ_find_sigid_algs(int signid, int *pdig_nid, int *ppkey_nid); // :3
int OBJ_find_sigid_by_algs(int *psignid, int dig_nid, int pkey_nid); // :3
int OBJ_add_sigid(int signid, int dig_id, int pkey_id); // :3
void OBJ_sigid_free(void); // :3

#define SN_ac_auditEntity SN_ac_auditIdentity // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3
