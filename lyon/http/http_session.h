#ifndef __LYON_HTTP_SESSION_H__
#define __LYON_HTTP_SESSION_H__

#include "http_protocol.h"
#include "lyon/socket.h"
#include "lyon/streams/socket_stream.h"
#include <memory>
namespace lyon::http {

/**
 * @brief 在SocketStream上进一步封装的Http会话类
 * 负责使用HttpParser将接收到的数据解析为其对应的解析类
 * 和将解析类转化为流数据输出
 */
class HttpSession : public SocketStream {
public:
    typedef std::shared_ptr<HttpSession> ptr;
    HttpSession(Socket::ptr sock) : SocketStream(sock) {}

    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr response);
};

} // namespace lyon::http

#endif
