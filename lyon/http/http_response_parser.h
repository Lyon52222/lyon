/**
 *
 * Copyright (c) 2010, Zed A. Shaw and Mongrel2 Project Contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the Mongrel2 Project, Zed A. Shaw, nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef http_response_parser_h
#define http_response_parser_h

#include "http_common.h"

typedef struct http_response_parser {
    int cs;
    size_t body_start;
    int content_len;
    int status;
    int chunked;
    int chunks_done;
    int close;
    size_t nread;
    size_t mark;
    size_t field_start;
    size_t field_len;

    void *data;

    field_cb http_field;
    element_cb reason_phrase;
    element_cb status_code;
    element_cb chunk_size;
    element_cb http_version;
    element_cb header_done;
    element_cb last_chunk;

} http_response_parser;

int http_response_parser_init(http_response_parser *parser);
int http_response_parser_finish(http_response_parser *parser);
size_t http_response_parser_execute(http_response_parser *parser,
                                    const char *data, size_t len, size_t off);
int http_response_parser_has_error(http_response_parser *parser);
int http_response_parser_is_finished(http_response_parser *parser);

#define http_response_parser_nread(parser) (parser)->nread

#endif
