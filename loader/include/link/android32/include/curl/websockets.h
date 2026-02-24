#ifndef CURLINC_WEBSOCKETS_H // :3
#define CURLINC_WEBSOCKETS_H // :3
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

struct curl_ws_frame { // :3
  int age;              /* zero */
  int flags;            /* See the CURLWS_* defines */
  curl_off_t offset;    /* the offset of this data into the frame */
  curl_off_t bytesleft; /* number of pending bytes left of the payload */
  size_t len;           /* size of the current data chunk */
};

/* flag bits */
#define CURLWS_TEXT       (1 << 0)
#define CURLWS_BINARY     (1 << 1)
#define CURLWS_CONT       (1 << 2)
#define CURLWS_CLOSE      (1 << 3)
#define CURLWS_PING       (1 << 4)
#define CURLWS_OFFSET     (1 << 5)

/*
 * NAME curl_ws_recv()
 *
 * DESCRIPTION
 *
 * Receives data from the websocket connection. Use after successful
 * curl_easy_perform() with CURLOPT_CONNECT_ONLY option.
 */ // :3
CURL_EXTERN CURLcode curl_ws_recv(CURL *curl, void *buffer, size_t buflen, // :3
                                  size_t *recv, // :3
                                  const struct curl_ws_frame **metap); // :3

/* flags for curl_ws_send() */
#define CURLWS_PONG       (1 << 6)

/*
 * NAME curl_ws_send()
 *
 * DESCRIPTION
 *
 * Sends data over the websocket connection. Use after successful
 * curl_easy_perform() with CURLOPT_CONNECT_ONLY option.
 */ // :3
CURL_EXTERN CURLcode curl_ws_send(CURL *curl, const void *buffer, // :3
                                  size_t buflen, size_t *sent, // :3
                                  curl_off_t fragsize, // :3
                                  unsigned int flags); // :3

/*
 * NAME curl_ws_start_frame()
 *
 * DESCRIPTION
 *
 * Buffers a websocket frame header with the given flags and length.
 * Errors when a previous frame is not complete, e.g. not all its
 * payload has been added.
 */ // :3
CURL_EXTERN CURLcode curl_ws_start_frame(CURL *curl, // :3
                                         unsigned int flags, // :3
                                         curl_off_t frame_len); // :3

/* bits for the CURLOPT_WS_OPTIONS bitmask: */
#define CURLWS_RAW_MODE   (1L << 0)
#define CURLWS_NOAUTOPONG (1L << 1)

CURL_EXTERN const struct curl_ws_frame *curl_ws_meta(CURL *curl);

#ifdef __cplusplus
}
#endif

#endif /* CURLINC_WEBSOCKETS_H */
