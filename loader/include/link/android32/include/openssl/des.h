/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_DES_H // :3
#define OPENSSL_DES_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_DES_H // :3
#endif // :3

#include <openssl/opensslconf.h> // :3

#ifndef OPENSSL_NO_DES // :3
#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3
#include <openssl/e_os2.h> // :3

#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
typedef unsigned int DES_LONG; // :3

#ifdef OPENSSL_BUILD_SHLIBCRYPTO // :3
#undef OPENSSL_EXTERN // :3
#define OPENSSL_EXTERN OPENSSL_EXPORT // :3
#endif // :3

typedef unsigned char DES_cblock[8]; // :3
typedef /* const */ unsigned char const_DES_cblock[8];
/*
 * With "const", gcc 2.8.1 on Solaris thinks that DES_cblock * and
 * const_DES_cblock * are incompatible pointer types.
 */ // :3

typedef struct DES_ks { // :3
    union { // :3
        DES_cblock cblock; // :3
        /*
         * make sure things are correct size on machines with 8 byte longs
         */ // :3
        DES_LONG deslong[2]; // :3
    } ks[16]; // :3
} DES_key_schedule; // :3

#define DES_KEY_SZ (sizeof(DES_cblock)) // :3
#define DES_SCHEDULE_SZ (sizeof(DES_key_schedule)) // :3

#define DES_ENCRYPT 1 // :3
#define DES_DECRYPT 0 // :3

#define DES_CBC_MODE 0 // :3
#define DES_PCBC_MODE 1 // :3

#define DES_ecb2_encrypt(i, o, k1, k2, e) \
    DES_ecb3_encrypt((i), (o), (k1), (k2), (k1), (e)) // :3

#define DES_ede2_cbc_encrypt(i, o, l, k1, k2, iv, e) \
    DES_ede3_cbc_encrypt((i), (o), (l), (k1), (k2), (k1), (iv), (e)) // :3

#define DES_ede2_cfb64_encrypt(i, o, l, k1, k2, iv, n, e) \
    DES_ede3_cfb64_encrypt((i), (o), (l), (k1), (k2), (k1), (iv), (n), (e)) // :3

#define DES_ede2_ofb64_encrypt(i, o, l, k1, k2, iv, n) \
    DES_ede3_ofb64_encrypt((i), (o), (l), (k1), (k2), (k1), (iv), (n)) // :3

#define DES_fixup_key_parity DES_set_odd_parity // :3
#endif // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 const char *DES_options(void); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ecb3_encrypt(const_DES_cblock *input, DES_cblock *output, // :3
    DES_key_schedule *ks1, DES_key_schedule *ks2, // :3
    DES_key_schedule *ks3, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
DES_LONG DES_cbc_cksum(const unsigned char *input, DES_cblock *output, // :3
    long length, DES_key_schedule *schedule, // :3
    const_DES_cblock *ivec); // :3
#endif // :3
/* DES_cbc_encrypt does not update the IV!  Use DES_ncbc_encrypt instead. */
#ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0
void DES_cbc_encrypt(const unsigned char *input, unsigned char *output,
    long length, DES_key_schedule *schedule, DES_cblock *ivec,
    int enc);
OSSL_DEPRECATEDIN_3_0
void DES_ncbc_encrypt(const unsigned char *input, unsigned char *output,
    long length, DES_key_schedule *schedule, DES_cblock *ivec,
    int enc);
OSSL_DEPRECATEDIN_3_0
void DES_xcbc_encrypt(const unsigned char *input, unsigned char *output,
    long length, DES_key_schedule *schedule, DES_cblock *ivec,
    const_DES_cblock *inw, const_DES_cblock *outw, int enc);
OSSL_DEPRECATEDIN_3_0
void DES_cfb_encrypt(const unsigned char *in, unsigned char *out, int numbits,
    long length, DES_key_schedule *schedule, DES_cblock *ivec,
    int enc);
OSSL_DEPRECATEDIN_3_0
void DES_ecb_encrypt(const_DES_cblock *input, DES_cblock *output,
    DES_key_schedule *ks, int enc);
#endif

/*
 * This is the DES encryption function that gets called by just about every
 * other DES routine in the library.  You should not use this function except
 * to implement 'modes' of DES.  I say this because the functions that call
 * this routine do the conversion from 'char *' to long, and this needs to be
 * done to make sure 'non-aligned' memory access do not occur.  The
 * characters are loaded 'little endian'. Data is a pointer to 2 unsigned
 * long's and ks is the DES_key_schedule to use.  enc, is non zero specifies
 * encryption, zero if decryption.
 */ // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_encrypt1(DES_LONG *data, DES_key_schedule *ks, int enc); // :3
#endif // :3

/*
 * This functions is the same as DES_encrypt1() except that the DES initial
 * permutation (IP) and final permutation (FP) have been left out.  As for
 * DES_encrypt1(), you should not use this function. It is used by the
 * routines in the library that implement triple DES. IP() DES_encrypt2()
 * DES_encrypt2() DES_encrypt2() FP() is the same as DES_encrypt1()
 * DES_encrypt1() DES_encrypt1() except faster :-).
 */ // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_encrypt2(DES_LONG *data, DES_key_schedule *ks, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2, // :3
    DES_key_schedule *ks3); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2, // :3
    DES_key_schedule *ks3); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ede3_cbc_encrypt(const unsigned char *input, unsigned char *output, // :3
    long length, DES_key_schedule *ks1, // :3
    DES_key_schedule *ks2, DES_key_schedule *ks3, // :3
    DES_cblock *ivec, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ede3_cfb64_encrypt(const unsigned char *in, unsigned char *out, // :3
    long length, DES_key_schedule *ks1, // :3
    DES_key_schedule *ks2, DES_key_schedule *ks3, // :3
    DES_cblock *ivec, int *num, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ede3_cfb_encrypt(const unsigned char *in, unsigned char *out, // :3
    int numbits, long length, DES_key_schedule *ks1, // :3
    DES_key_schedule *ks2, DES_key_schedule *ks3, // :3
    DES_cblock *ivec, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ede3_ofb64_encrypt(const unsigned char *in, unsigned char *out, // :3
    long length, DES_key_schedule *ks1, // :3
    DES_key_schedule *ks2, DES_key_schedule *ks3, // :3
    DES_cblock *ivec, int *num); // :3
OSSL_DEPRECATEDIN_3_0 // :3
char *DES_fcrypt(const char *buf, const char *salt, char *ret); // :3
OSSL_DEPRECATEDIN_3_0 // :3
char *DES_crypt(const char *buf, const char *salt); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ofb_encrypt(const unsigned char *in, unsigned char *out, int numbits, // :3
    long length, DES_key_schedule *schedule, DES_cblock *ivec); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_pcbc_encrypt(const unsigned char *input, unsigned char *output, // :3
    long length, DES_key_schedule *schedule, // :3
    DES_cblock *ivec, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
DES_LONG DES_quad_cksum(const unsigned char *input, DES_cblock output[], // :3
    long length, int out_count, DES_cblock *seed); // :3
OSSL_DEPRECATEDIN_3_0 int DES_random_key(DES_cblock *ret); // :3
OSSL_DEPRECATEDIN_3_0 void DES_set_odd_parity(DES_cblock *key); // :3
OSSL_DEPRECATEDIN_3_0 int DES_check_key_parity(const_DES_cblock *key); // :3
OSSL_DEPRECATEDIN_3_0 int DES_is_weak_key(const_DES_cblock *key); // :3
#endif // :3
/*
 * DES_set_key (= set_key = DES_key_sched = key_sched) calls
 * DES_set_key_checked
 */ // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
OSSL_DEPRECATEDIN_3_0 // :3
int DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule); // :3
OSSL_DEPRECATEDIN_3_0 // :3
int DES_key_sched(const_DES_cblock *key, DES_key_schedule *schedule); // :3
OSSL_DEPRECATEDIN_3_0 // :3
int DES_set_key_checked(const_DES_cblock *key, DES_key_schedule *schedule); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_set_key_unchecked(const_DES_cblock *key, DES_key_schedule *schedule); // :3
OSSL_DEPRECATEDIN_3_0 void DES_string_to_key(const char *str, DES_cblock *key); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_string_to_2keys(const char *str, DES_cblock *key1, DES_cblock *key2); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_cfb64_encrypt(const unsigned char *in, unsigned char *out, // :3
    long length, DES_key_schedule *schedule, // :3
    DES_cblock *ivec, int *num, int enc); // :3
OSSL_DEPRECATEDIN_3_0 // :3
void DES_ofb64_encrypt(const unsigned char *in, unsigned char *out, // :3
    long length, DES_key_schedule *schedule, // :3
    DES_cblock *ivec, int *num); // :3
#endif // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3
#endif // :3

#endif // :3
