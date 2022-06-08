#ifndef __LYON_HTTP_SERVER_H__
#define __LYON_HTTP_SERVER_H__
#include "http_servlet.h"
#include "lyon/tcp_server.h"

namespace lyon::http {
/**
 * @brief 在TcpServer的基础上进一步封装，将Accept的socket封装成HttpSession。
 * 并且将管理后续HttpSession接收和发送的报文。
 * 使用对应的Servlet进行处理。
 */
class HttpServer : public TcpServer {
public:
    typedef std::shared_ptr<HttpServer> ptr;

    using TcpServer::TcpServer;

    HttpServer(IOManager *worker = IOManager::GetCurrentIOManager(),
               IOManager *ioworker = IOManager::GetCurrentIOManager(),
               IOManager *acceptWorker = IOManager::GetCurrentIOManager(),
               bool connection = true);

    virtual void handleClient(Socket::ptr sock) override;

    void setServletDispatch(Servlet::ptr servlet) { m_servlet = servlet; }

    Servlet::ptr getServletDispatch() const { return m_servlet; }

private:
    bool m_connection;
    Servlet::ptr m_servlet;
};

} // namespace lyon::http
#endif
