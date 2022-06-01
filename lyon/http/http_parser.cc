#include "http_parser.h"
#include "http11_parser.h"
#include "httpclient_parser.h"
#include "lyon/config.h"
#include "lyon/log.h"
#include <cstdint>
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

static uint64_t s_http_request_buffer_size = 0;
static uint64_t s_http_request_max_body_size = 0;
static uint64_t s_http_response_buffer_size = 0;
static uint64_t s_http_response_max_body_size = 0;

//匿名内部类可以防止外部使用
namespace {
struct _HttpSizeIniter {
    _HttpSizeIniter() {
        s_http_request_buffer_size = g_http_request_buffer_size->getVal();
        s_http_request_max_body_size = g_http_request_max_body_size->getVal();
        s_http_response_buffer_size = g_http_response_buffer_size->getVal();
        s_http_response_max_body_size = g_http_response_max_body_size->getVal();

        g_http_request_buffer_size->addOnChange(
            [](const uint64_t &ov, const uint64_t &nv) {
                s_http_request_buffer_size = nv;
            });

        g_http_request_max_body_size->addOnChange(
            [](const uint64_t &ov, const uint64_t &nv) {
                s_http_request_max_body_size = nv;
            });

        g_http_response_buffer_size->addOnChange(
            [](const uint64_t &ov, const uint64_t &nv) {
                s_http_response_buffer_size = nv;
            });

        g_http_response_max_body_size->addOnChange(
            [](const uint64_t &ov, const uint64_t &nv) {
                s_http_response_max_body_size = nv;
            });
    }
};

static _HttpSizeIniter _s_http_size_initer;

} // namespace

uint64_t HttpRequestParser::GetBufferSize() {
    return s_http_request_buffer_size;
}

uint64_t HttpRequestParser::GetMaxBodySize() {
    return s_http_request_max_body_size;
}

uint64_t HttpResponseParser::GetBufferSize() {
    return s_http_response_buffer_size;
}

uint64_t HttpResponseParser::GetMaxBodySize() {
    return s_http_response_max_body_size;
}

void on_request_http_field(void *data, const char *field, size_t flen,
                           const char *value, size_t vlen) {
    if (flen == 0) {
        LYON_LOG_ERROR(g_logger)
            << "HttpRequestParser-on_request_http_filed error "
               ": filed length = 0";
        return;
    }
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setHeader(std::string(field, flen),
                                         std::string(value, vlen));
}

void on_request_method(void *data, const char *at, size_t length) {
    HttpMethod method = String2HttpMethod(at);
    if (method == HttpMethod::HTTP_METHOD_INVALID) {
        LYON_LOG_INFO(g_logger) << "HttpRequestParser-on_request_method error "
                                   ": Invalid method, method = "
                                << at;
    }
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setMethod(method);
}
void on_request_uri(void *data, const char *at, size_t length) {}

void on_request_fragment(void *data, const char *at, size_t length) {
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setFragment(std::string(at, length));
}

void on_request_path(void *data, const char *at, size_t length) {
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setPath(std::string(at, length));
}
void on_request_query_string(void *data, const char *at, size_t length) {
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setQuery(std::string(at, length));
}

void on_request_http_version(void *data, const char *at, size_t length) {
    uint8_t v = 0;
    if (strncmp(at, "HTTP/1.0", length) == 0) {
        v = 0x10;
    } else if (strncmp(at, "HTTP/1.1", length) == 0) {
        v = 0x11;
    } else {
        LYON_LOG_ERROR(g_logger)
            << "HttpRequestParser-on_request_http_version error "
               ": Unknown http version, version = "
            << at;
        return;
    }
    HttpRequestParser *request_parser = static_cast<HttpRequestParser *>(data);
    request_parser->getData()->setVersion(v);
}

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

size_t HttpRequestParser::excute(const char *data, size_t len, size_t offset) {
    return http_parser_execute(&m_parser, data, len, offset);
}

int HttpRequestParser::hasError() { return http_parser_has_error(&m_parser); }

int HttpRequestParser::isFinish() { return http_parser_is_finished(&m_parser); }

uint64_t HttpRequestParser::getContentLength() const {
    return m_data->getHeaderAs<uint64_t>("Content-Length", 0);
}
void on_response_http_field(void *data, const char *field, size_t flen,
                            const char *value, size_t vlen) {
    if (flen == 0) {
        LYON_LOG_ERROR(g_logger)
            << "HttpResponseParser-on_response_http_filed error "
               ": filed length = 0";
        return;
    }
    HttpResponseParser *response_parser =
        static_cast<HttpResponseParser *>(data);
    response_parser->getData()->setHeader(std::string(field, flen),
                                          std::string(value, vlen));
}

void on_response_reason_phrase(void *data, const char *at, size_t length) {
    HttpResponseParser *response_parser =
        static_cast<HttpResponseParser *>(data);
    response_parser->getData()->setReason(std::string(at, length));
}
void on_response_status_code(void *data, const char *at, size_t length) {}

void on_response_chunk_size(void *data, const char *at, size_t length) {}

void on_response_http_version(void *data, const char *at, size_t length) {
    uint8_t v = 0;
    if (strncmp(at, "HTTP/1.0", length) == 0) {
        v = 0x10;
    } else if (strncmp(at, "HTTP/1.1", length) == 0) {
        v = 0x11;
    } else {
        LYON_LOG_ERROR(g_logger)
            << "HttpRequestParser-on_request_http_version error "
               ": Unknown http version, version = "
            << at;
        return;
    }
    HttpResponseParser *response_parser =
        static_cast<HttpResponseParser *>(data);
    response_parser->getData()->setVersion(v);
}

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

int HttpResponseParser::excute(const char *data, size_t len, size_t offset) {
    return httpclient_parser_execute(&m_parser, data, len, offset);
}

int HttpResponseParser::hasError() {
    return httpclient_parser_has_error(&m_parser);
}

int HttpResponseParser::isFinish() {
    return httpclient_parser_is_finished(&m_parser);
}

uint64_t HttpResponseParser::getContentLength() const {
    return getData()->getHeaderAs<uint64_t>("content-length", 0);
}
} // namespace http
} // namespace lyon
