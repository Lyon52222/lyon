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

        // //这里是不丢弃已解析数据的代码
        // size_t excuted = request_parser->excute(buf, offset + readed,
        // offset); offset += excuted;

        //这里如果不需要已经解析过的内容的话是可以直接丢弃的。
        readed += offset;
        size_t excuted = request_parser->excute(buf, readed, 0);
        //将解析完了的内容丢弃
        memmove(buf, buf + excuted, readed - excuted);
        offset = readed - excuted;

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
        //如果解析完头部后剩余的内容比content_length小
        size_t readed = 0;
        if (offset < content_length) {
            //剩余内容比body小/说明已经读取了部分的body。
            memcpy(&body[0], buf, offset);
            readed = offset;
        } else {
            //剩余内容比body大/说明整个body已经读取完了
            memcpy(&body[0], buf, content_length);
            readed = content_length;
        }
        content_length -= readed;
        if (readFixSize(&body[readed], content_length) <= 0) {
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
