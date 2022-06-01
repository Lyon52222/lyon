#ifndef __LYON_HTTP_HTTP_SERVER_H__
#define __LYON_HTTP_HTTP_SERVER_H__
#include "lyon/tcp_server.h"

namespace lyon {
namespace http {
class HttpServer : public TcpServer {
public:
    typedef std::shared_ptr<HttpServer> ptr;

    using TcpServer::TcpServer;

    HttpServer(IOManager *worker = IOManager::GetCurrentIOManager(),
               IOManager *ioworker = IOManager::GetCurrentIOManager(),
               IOManager *acceptWorker = IOManager::GetCurrentIOManager(),
               bool connection = true);

    virtual void handleClient(Socket::ptr sock) override;

private:
    bool m_connection;
};

} // namespace http
} // namespace lyon
#endif
