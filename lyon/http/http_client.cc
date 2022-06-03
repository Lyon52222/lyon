#include "http_client.h"
#include <cstdint>
#include <http_parser.h>
#include <lyon/log.h>
namespace lyon {
namespace http {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

HttpResponse::ptr HttpClient::recvResponse() {
    HttpResponseParser::ptr response_parser(new HttpResponseParser());
    uint64_t response_size = HttpResponseParser::GetBufferSize();
    std::shared_ptr<char> buffer(new char[response_size],
                                 [](char *ptr) { delete[] ptr; });
    char *buf = buffer.get();
    size_t offset = 0;
    do {
        size_t readed = read(buf + offset, response_size - offset);
        if (readed <= 0) {
            close();
            return nullptr;
        }
        // //不丢弃已解析数据
        // size_t excuted = response_parser->excute(buf, offset + readed,
        // offset); offset += excuted;

        readed += offset;
        // buf[readed] = '\0';
        size_t excuted = response_parser->excute(buf, readed + offset, 0);
        // 丢弃已解析数据
        memmove(buf, buf + excuted, readed - excuted);
        offset = readed - excuted;
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
        uint64_t content_length = response_parser->getContentLength();
        if (content_length > 0) {
            body.resize(content_length);
            size_t readed = 0;
            if (offset < content_length) {
                memcpy(&body[0], buf, offset);
                readed = offset;
            } else {
                memcpy(&body[0], buf, content_length);
                readed = content_length;
            }
            content_length -= readed;
            if (readFixSize(&body[readed], content_length) <= 0) {
                close();
                return nullptr;
            }
        }

    } else {
        // body是分块传输的
        // chunk的结构是 [chunk size]\r\n[chunk data]\r\n[chunk size]\r\n[chunk
        // data]
        do {
            //解析分块
            do {
                size_t readed = read(buf + offset, response_size - offset);
                if (readed <= 0) {
                    close();
                    return nullptr;
                }
                readed += offset;
                size_t executed = response_parser->excute(buf, readed, 0);
                offset = readed - executed;
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
            size_t chunk_size = response_parser->getChunkSize();
            if (chunk_size + 2 <= offset) {
                // buf中已经包含了一个完整的chunk了。 直接将chunk复制过去。
                body.append(buf, chunk_size);
                memmove(buf, buf + chunk_size + 2, offset - chunk_size - 2);
                offset -= chunk_size + 2;
            } else {
                //将已经读取过的部分复制过去，然后读取剩余的部分
                body.append(buf, offset);
                size_t left = chunk_size - offset + 2;
                while (left > 0) {
                    size_t readed =
                        read(buf, left < response_size ? left : response_size);
                    if (readed <= 0) {
                        close();
                        return nullptr;
                    }
                    body.append(buf, readed);
                    left -= readed;
                }
                body.resize(body.size() - 2);
                offset = 0;
            }

        } while (!response_parser->isChunksDone());
    }
    if (!body.empty()) {
        response_parser->getData()->setBody(body);
    }

    return response_parser->getData();
}

int HttpClient::sendRequest(HttpRequest::ptr request) {
    std::string str = request->toString();
    return writeFixSize(str.c_str(), str.size());
}

} // namespace http
} // namespace lyon
