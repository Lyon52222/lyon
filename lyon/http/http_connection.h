#ifndef __LYON_HTTP_HTTP_CONNECTION_H__
#define __LYON_HTTP_HTTP_CONNECTION_H__
#include "http_protocol.h"
#include "lyon/mutex.h"
#include "lyon/streams/socket_stream.h"
#include "lyon/uri.h"
#include <atomic>
#include <cstdint>
#include <list>
#include <lyon/util.h>
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

        POOL_GET_CONN_FAIL,
        POOL_INVALID_SOCK
    };

    HttpResult(Error _result, HttpResponse::ptr _response,
               const std::string &_error)
        : result(_result), response(_response), error(_error) {}

    Error result;
    HttpResponse::ptr response;
    std::string error;
    std::string toString() const;
};

/**
 * @brief 客户端比较简单，只需要接收和发送报文，不需要监听。
 */
class HttpConnection : public SocketStream {
public:
    typedef std::shared_ptr<HttpConnection> ptr;
    HttpConnection(Socket::ptr sock) : SocketStream(sock) {
        m_createTime = GetCurrentTimeMS();
    }

    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr request);

    static HttpResult::ptr DoRequest(HttpRequest::ptr request, Uri::ptr uri,
                                     uint64_t timeout_ms);

    static HttpResult::ptr
    DoRequest(HttpMethod method, Uri::ptr uri,
              const std::map<std::string, std::string> &headers,
              const std::string &body, uint64_t timeout_ms);

    uint64_t getCreateTime() const { return m_createTime; }
    uint32_t incRequest() { return ++m_request; }
    uint32_t decRequest() { return --m_request; }
    uint32_t getRequest() const { return m_request; }

private:
    uint32_t m_request;
    uint64_t m_createTime;
};

class HttpConnectionPool {
public:
    typedef std::shared_ptr<HttpConnectionPool> ptr;
    typedef Mutex MutexType;

    HttpConnectionPool(const std::string &host, const std::string &vhost,
                       uint16_t port, uint32_t maxSize, uint32_t maxLiveTime,
                       uint32_t maxRequest);

    HttpConnectionPool::ptr Create(const std::string &host,
                                   const std::string &vhost, uint16_t port,
                                   uint32_t maxSize, uint32_t maxLiveTime,
                                   uint32_t maxRequest);

    HttpConnection::ptr getConnection();

    HttpResult::ptr doRequest(HttpRequest::ptr request, uint64_t timeout_ms);

private:
    static void ReleasePtr(HttpConnection *ptr, HttpConnectionPool *pool);

private:
    /**
     * @m_host 域名
     */
    std::string m_host;
    /**
     * @m_vhost 备用域名
     */
    std::string m_vhost;

    /**
     * @m_port 端口号
     */
    uint16_t m_port;
    /**
     * @m_maxSize 最大链接数
     */
    uint32_t m_maxSize;
    /**
     * @m_maxLiveTime 每条链接的最大生存时间
     */
    uint32_t m_maxLiveTime;
    /**
     * @m_maxRequest 每条链接的最大请求数
     */
    uint32_t m_maxRequest;

    std::list<HttpConnection *> m_connections;

    std::atomic<uint32_t> m_total = {0};

    MutexType m_mutex;
};

} // namespace http
} // namespace lyon

#endif
