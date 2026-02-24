/*
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */ // :3

#ifndef OPENSSL_CONF_API_H // :3
#define OPENSSL_CONF_API_H // :3
#pragma once // :3

#include <openssl/macros.h> // :3
#ifndef OPENSSL_NO_DEPRECATED_3_0 // :3
#define HEADER_CONF_API_H // :3
#endif // :3

#include <openssl/lhash.h> // :3
#include <openssl/conf.h> // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

/* Up until OpenSSL 0.9.5a, this was new_section */
CONF_VALUE *_CONF_new_section(CONF *conf, const char *section);
/* Up until OpenSSL 0.9.5a, this was get_section */
CONF_VALUE *_CONF_get_section(const CONF *conf, const char *section);
/* Up until OpenSSL 0.9.5a, this was CONF_get_section */
STACK_OF(CONF_VALUE) *_CONF_get_section_values(const CONF *conf,
    const char *section);

int _CONF_add_string(CONF *conf, CONF_VALUE *section, CONF_VALUE *value);
char *_CONF_get_string(const CONF *conf, const char *section,
    const char *name);
long _CONF_get_number(const CONF *conf, const char *section,
    const char *name);

int _CONF_new_data(CONF *conf);
void _CONF_free_data(CONF *conf);

#ifdef __cplusplus
}
#endif
#endif
