#ifndef __LYON_HTTP_HTTP_CLIENT_H__
#define __LYON_HTTP_HTTP_CLIENT_H__
#include "lyon/streams/socket_stream.h"
#include <http_protocol.h>
namespace lyon {
namespace http {

/**
 * @brief 客户端比较简单，只需要接收和发送报文，不需要监听。
 */
class HttpClient : public SocketStream {
public:
    typedef std::shared_ptr<HttpClient> ptr;
    HttpClient(Socket::ptr sock) : SocketStream(sock) {}

    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr request);
};

} // namespace http
} // namespace lyon

#endif
