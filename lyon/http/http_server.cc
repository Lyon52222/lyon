#include "http_server.h"
#include "http_session.h"
#include "lyon/log.h"
namespace lyon::http {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

HttpServer::HttpServer(IOManager *worker, IOManager *ioworker,
                       IOManager *acceptWorker, bool connection)
    : TcpServer(worker, ioworker, acceptWorker), m_connection(connection) {}

void HttpServer::handleClient(Socket::ptr sock) {
    HttpSession::ptr session(new HttpSession(sock));
    do {
        HttpRequest::ptr request = session->recvRequest();

        if (!request) {
            LYON_LOG_DEBUG(g_logger) << "HttpServer::handleClient Bad Request";
            break;
        }

        HttpResponse::ptr response(new HttpResponse(
            request->getVersion(), (!m_connection || !session->isConnected())));

        if (m_servlet->handle(request, response, session) == 0)
            session->sendResponse(response);

        //连接是否是长链接
        m_connection = request->isConnection();

        if (!m_connection || !session->isConnected()) {
            break;
        }
    } while (true);
    session->close();
}

} // namespace lyon::http
