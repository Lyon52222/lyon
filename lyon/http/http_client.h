#ifndef __LYON_HTTP_HTTP_CLIENT_H__
#define __LYON_HTTP_HTTP_CLIENT_H__
#include "http_protocol.h"
#include "lyon/streams/socket_stream.h"
#include "lyon/uri.h"
#include <memory>
namespace lyon {
namespace http {

struct HttpResult {
    typedef std::shared_ptr<HttpResult> ptr;
    enum class Error {
        //正常
        OK = 0,
        INVALID_URL,
        INVALID_HOST,
        CONNECT_FAIL,
        SEND_CLOSE_BY_PEER,
        SEND_SOCKET_ERROR,
        TIMEOUT,
        CREATE_SOCKET_ERROR,
    };

    int result;
    HttpResponse::ptr response;
    std::string error;
    std::string toString() const;
};

/**
 * @brief 客户端比较简单，只需要接收和发送报文，不需要监听。
 */
class HttpClient : public SocketStream {
public:
    typedef std::shared_ptr<HttpClient> ptr;
    HttpClient(Socket::ptr sock) : SocketStream(sock) {}

    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr request);

    static HttpResult::ptr DoRequest(HttpRequest::ptr request, Uri::ptr uri,
                                     uint64_t timeout_ms);
};

class HttpConnectionPool {};

} // namespace http
} // namespace lyon

#endif
