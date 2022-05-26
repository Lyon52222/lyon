#ifndef __LYON_HTTP_HTTP_PARSER_H__
#define __LYON_HTTP_HTTP_PARSER_H__

#include "http.h"
#include "http11_common.h"
#include "http11_parser.h"
#include "httpclient_parser.h"
#include <memory>
namespace lyon {
namespace http {

class HttpRequestParser {
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
    HttpRequestParser();

    int finish();
    size_t excute(const char *data, size_t len);
    int hasError();
    int isFInish();
    HttpRequest::ptr getData() const { return m_data; }

private:
    HttpRequest::ptr m_data;
    http_parser m_parser;
};

class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;
    HttpResponseParser();

    int finish();
    int excute(const char *data, size_t len);
    int hasError();
    int isFinish();
    HttpResponse::ptr getData() const { return m_data; }

private:
    HttpResponse::ptr m_data;
    httpclient_parser m_parser;
};

} // namespace http

} // namespace lyon

#endif // !__LYON_HTTP_HTTP_PARSER_H__
