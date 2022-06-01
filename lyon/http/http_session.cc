#include "http_session.h"
#include "http_parser.h"
#include "iostream"
#include "lyon/bytearray.h"
#include <cstdint>
#include <sys/types.h>
namespace lyon {
namespace http {

HttpRequest::ptr HttpSession::recvRequest() {
    HttpRequestParser::ptr request_parser(new HttpRequestParser());
    u_int64_t request_size = HttpRequestParser::GetBufferSize();
    std::shared_ptr<char> buffer(new char[request_size],
                                 [](char *ptr) { delete[] ptr; });
    char *buf = buffer.get();
    size_t offset = 0;
    do {
        size_t readed = read(buf + offset, request_size - offset);
        if (readed <= 0) {
            close();
            return nullptr;
        }
        request_parser->excute(buf, offset + readed, offset);
        offset += readed;
        if (request_parser->hasError()) {
            close();
            return nullptr;
        }
        if (request_parser->isFinish()) {
            break;
        }
        if (offset == request_size) {
            close();
            return nullptr;
        }
    } while (true);

    uint64_t content_length = request_parser->getContentLength();

    if (content_length > 0) {
        std::string body;
        body.resize(content_length);
        if (readFixSize(&body[0], content_length) <= 0) {
            close();
            return nullptr;
        }

        request_parser->getData()->setBody(body);
    }
    return request_parser->getData();
}

int HttpSession::sendResponse(HttpResponse::ptr response) {
    std::string str = response->toString();
    return writeFixSize(str.c_str(), str.size());
}

} // namespace http
} // namespace lyon
