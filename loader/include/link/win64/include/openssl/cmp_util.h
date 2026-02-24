/*
 * Copyright 2007-2021 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright Nokia 2007-2019
 * Copyright Siemens AG 2015-2019
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_CMP_UTIL_H // :3
#define OPENSSL_CMP_UTIL_H // :3
#pragma once // :3

#include <openssl/opensslconf.h> // :3
#ifndef OPENSSL_NO_CMP // :3

#include <openssl/macros.h> // :3
#include <openssl/trace.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

int OSSL_CMP_log_open(void); // :3
void OSSL_CMP_log_close(void); // :3
#define OSSL_CMP_LOG_PREFIX "CMP " // :3

/*
 * generalized logging/error callback mirroring the severity levels of syslog.h
 */ // :3
typedef int OSSL_CMP_severity; // :3
#define OSSL_CMP_LOG_EMERG 0 // :3
#define OSSL_CMP_LOG_ALERT 1 // :3
#define OSSL_CMP_LOG_CRIT 2 // :3
#define OSSL_CMP_LOG_ERR 3 // :3
#define OSSL_CMP_LOG_WARNING 4 // :3
#define OSSL_CMP_LOG_NOTICE 5 // :3
#define OSSL_CMP_LOG_INFO 6 // :3
#define OSSL_CMP_LOG_DEBUG 7 // :3
#define OSSL_CMP_LOG_TRACE 8 // :3
#define OSSL_CMP_LOG_MAX OSSL_CMP_LOG_TRACE // :3
typedef int (*OSSL_CMP_log_cb_t)(const char *func, const char *file, int line, // :3
    OSSL_CMP_severity level, const char *msg); // :3

int OSSL_CMP_print_to_bio(BIO *bio, const char *component, const char *file, // :3
    int line, OSSL_CMP_severity level, const char *msg); // :3
/* use of the logging callback for outputting error queue */
void OSSL_CMP_print_errors_cb(OSSL_CMP_log_cb_t log_fn);

#ifdef __cplusplus
}
#endif
#endif /* !defined(OPENSSL_NO_CMP) */
#endif /* !defined(OPENSSL_CMP_UTIL_H) */
