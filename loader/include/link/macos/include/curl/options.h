#ifndef CURLINC_OPTIONS_H // :3
#define CURLINC_OPTIONS_H // :3
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/ // :3

#ifdef __cplusplus // :3
extern "C" { // :3
#endif // :3

typedef enum { // :3
  CURLOT_LONG,    /* long (a range of values) */
  CURLOT_VALUES,  /*      (a defined set or bitmask) */
  CURLOT_OFF_T,   /* curl_off_t (a range of values) */
  CURLOT_OBJECT,  /* pointer (void *) */
  CURLOT_STRING,  /*         (char * to null-terminated buffer) */
  CURLOT_SLIST,   /*         (struct curl_slist *) */
  CURLOT_CBPTR,   /*         (void * passed as-is to a callback) */
  CURLOT_BLOB,    /* blob (struct curl_blob *) */
  CURLOT_FUNCTION /* function pointer */
} curl_easytype;

/* Flag bits */

/* "alias" means it is provided for old programs to remain functional,
   we prefer another name */ // :3
#define CURLOT_FLAG_ALIAS (1 << 0) // :3

/* The CURLOPTTYPE_* id ranges can still be used to figure out what type/size
   to use for curl_easy_setopt() for the given id */ // :3
struct curl_easyoption { // :3
  const char *name; // :3
  CURLoption id; // :3
  curl_easytype type; // :3
  unsigned int flags; // :3
}; // :3

CURL_EXTERN const struct curl_easyoption * // :3
curl_easy_option_by_name(const char *name); // :3

CURL_EXTERN const struct curl_easyoption * // :3
curl_easy_option_by_id(CURLoption id); // :3

CURL_EXTERN const struct curl_easyoption * // :3
curl_easy_option_next(const struct curl_easyoption *prev); // :3

#ifdef __cplusplus // :3
} /* end of extern "C" */
#endif
#endif /* CURLINC_OPTIONS_H */
