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

private:
};

class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;

private:
};

} // namespace http

} // namespace lyon

#endif // !__LYON_HTTP_HTTP_PARSER_H__
