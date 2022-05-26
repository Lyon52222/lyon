#include "http_parser.h"
#include "lyon/config.h"
#include "lyon/log.h"
#include <cstdint>
#include <http.h>
#include <http11_parser.h>
#include <httpclient_parser.h>
namespace lyon {
namespace http {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

static ConfigVar<uint64_t>::ptr g_http_request_buffer_size =
    Config::SetConfig("http.request.buffer_size", (uint64_t)(4 * 1024),
                      "http request buffer size");

static ConfigVar<uint64_t>::ptr g_http_request_max_body_size =
    Config::SetConfig("http.request.max_body_size",
                      (uint64_t)(64 * 1024 * 1024),
                      "http request max body size");

static ConfigVar<uint64_t>::ptr g_http_response_buffer_size =
    Config::SetConfig("http.response.buffer_size", (uint64_t)(4 * 1024),
                      "http response buffer size");

static ConfigVar<uint64_t>::ptr g_http_response_max_body_size =
    Config::SetConfig("http.response.max_body_size",
                      (uint64_t)(64 * 1024 * 1024),
                      "http response max body size");

void on_request_http_field(void *data, const char *field, size_t flen,
                           const char *value, size_t vlen) {}

void on_request_method(void *data, const char *at, size_t length) {
    HttpMethod method = String2HttpMethod(at);
    if (method == HttpMethod::HTTP_METHOD_INVALID) {
        LYON_LOG_ERROR(g_logger) << "HttpRequestParser-on_request_method error "
                                    ": Invalid method, method = "
                                 << at;
        return;
    }
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setMethod(method);
}
void on_request_uri(void *data, const char *at, size_t length) {}
void on_request_fragment(void *data, const char *at, size_t length) {}
void on_request_path(void *data, const char *at, size_t length) {}
void on_request_query_string(void *data, const char *at, size_t length) {}
void on_request_http_version(void *data, const char *at, size_t length) {}
void on_request_header_done(void *data, const char *at, size_t length) {}

HttpRequestParser::HttpRequestParser() {
    m_data.reset(new HttpRequest());
    http_parser_init(&m_parser);
    m_parser.http_field = on_request_http_field;
    m_parser.request_method = on_request_method;
    m_parser.request_uri = on_request_uri;
    m_parser.fragment = on_request_fragment;
    m_parser.request_path = on_request_path;
    m_parser.query_string = on_request_query_string;
    m_parser.http_version = on_request_http_version;
    m_parser.header_done = on_request_header_done;
    m_parser.data = this;
}

int HttpRequestParser::finish() { return http_parser_finish(&m_parser); }

size_t HttpRequestParser::excute(const char *data, size_t len) {
    return http_parser_execute(&m_parser, data, len, 0);
}

int HttpRequestParser::hasError() { return http_parser_has_error(&m_parser); }

int HttpRequestParser::isFInish() { return http_parser_is_finished(&m_parser); }

void on_response_http_field(void *data, const char *field, size_t flen,
                            const char *value, size_t vlen) {}
void on_response_reason_phrase(void *data, const char *at, size_t length) {}
void on_response_status_code(void *data, const char *at, size_t length) {}
void on_response_chunk_size(void *data, const char *at, size_t length) {}

void on_response_http_version(void *data, const char *at, size_t length) {}

void on_response_header_done(void *data, const char *at, size_t length) {}

void on_request_last_chunk(void *data, const char *at, size_t length) {}

HttpResponseParser::HttpResponseParser() {
    m_data.reset(new HttpResponse());
    httpclient_parser_init(&m_parser);
    m_parser.http_field = on_response_http_field;
    m_parser.reason_phrase = on_response_reason_phrase;
    m_parser.status_code = on_response_status_code;
    m_parser.chunk_size = on_response_chunk_size;
    m_parser.http_version = on_response_http_version;
    m_parser.header_done = on_response_header_done;
    m_parser.last_chunk = on_request_last_chunk;
    m_parser.data = this;
}

int HttpResponseParser::finish() { return httpclient_parser_finish(&m_parser); }

int HttpResponseParser::excute(const char *data, size_t len) {
    return httpclient_parser_execute(&m_parser, data, len, 0);
}

int HttpResponseParser::hasError() {
    return httpclient_parser_has_error(&m_parser);
}

int HttpResponseParser::isFinish() {
    return httpclient_parser_is_finished(&m_parser);
}

} // namespace http
} // namespace lyon
