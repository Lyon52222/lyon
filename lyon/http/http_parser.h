#ifndef __LYON_HTTP_HTTP_PARSER_H__
#define __LYON_HTTP_HTTP_PARSER_H__

#include "http_common.h"
#include "http_protocol.h"
#include "http_request_parser.h"
#include "http_response_parser.h"
#include <cstdint>
#include <memory>
namespace lyon {
namespace http {

/**
 * @brief 请求协议的解析类，负责将请求报文解析成其对应的解析类
 */
class HttpRequestParser {
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
    HttpRequestParser();

    int finish();
    size_t excute(const char *data, size_t len, size_t offset);
    int hasError();
    int isFinish();
    HttpRequest::ptr getData() const { return m_data; }
    uint64_t getContentLength() const;

    static uint64_t GetBufferSize();
    static uint64_t GetMaxBodySize();

private:
    HttpRequest::ptr m_data;
    http_request_parser m_parser;
};

/**
 * @brief 响应协议的解析类，负责将响应报文解析成其对应的解析类
 */
class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;
    HttpResponseParser();

    int finish();
    int excute(const char *data, size_t len, size_t offset, bool chunk = false);
    int hasError();
    int isFinish();
    HttpResponse::ptr getData() const { return m_data; }
    uint64_t getContentLength() const;

    int isChunked() const;
    int getChunkSize() const;
    int isChunksDone() const;

    static uint64_t GetBufferSize();
    static uint64_t GetMaxBodySize();

private:
    HttpResponse::ptr m_data;
    http_response_parser m_parser;
};

} // namespace http

} // namespace lyon

#endif // !__LYON_HTTP_HTTP_PARSER_H__
