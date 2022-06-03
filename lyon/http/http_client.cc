#include "http_client.h"
#include <cstdint>
#include <http_parser.h>
namespace lyon {
namespace http {

HttpResponse::ptr HttpClient::recvResponse() {
    HttpResponseParser::ptr response_parser(new HttpResponseParser());
    uint64_t response_size = HttpResponseParser::GetBufferSize();
    std::shared_ptr<char> buffer(new char[response_size],
                                 [](char *ptr) { delete[] ptr; });
    char *buf = buffer.get();
    size_t offset = 0;
    // TODO: 这里始终还是有些细节要处理
    do {
        size_t readed = read(buf + offset, response_size - offset);
        if (readed <= 0) {
            close();
            return nullptr;
        }
        //不丢弃已解析数据
        size_t excuted = response_parser->excute(buf, offset + readed, offset);
        offset += excuted;

        //丢弃已解析数据
        // size_t excuted = response_parser->excute(buf, 0, readed);
        // memmove(buf, buf + excuted, readed - excuted);
        // offset = readed - excuted;
        if (response_parser->hasError()) {
            close();
            return nullptr;
        }
        if (response_parser->isFinish()) {
            break;
        }
        if (offset == response_size) {
            close();
            return nullptr;
        }
    } while (true);

    std::string body;

    if (!response_parser->isChunked()) {
        // body 不是分块传输的，直接按照Content-Length指定的长度读取就行了。
        uint64_t content_length = response_parser->getContentLength();
        if (content_length > 0) {
            body.resize(content_length);
            if (readFixSize(&body[0], content_length) <= 0) {
                close();
                return nullptr;
            }
            response_parser->getData()->setBody(body);
        }

    } else {
        // body是分块传输的
    }
    return response_parser->getData();
}

int HttpClient::sendRequest(HttpRequest::ptr request) {}

} // namespace http
} // namespace lyon
