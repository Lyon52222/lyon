#ifndef __LYON_HTTP_HTTP_SESSION_H__
#define __LYON_HTTP_HTTP_SESSION_H__

#include "http_protocol.h"
#include "lyon/socket.h"
#include "lyon/streams/socket_stream.h"
#include <memory>
namespace lyon {
namespace http {

class HttpSession : public SocketStream {
public:
    typedef std::shared_ptr<HttpSession> ptr;
    HttpSession(Socket::ptr sock) : SocketStream(sock) {}

    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr response);
};

} // namespace http
} // namespace lyon

#endif
