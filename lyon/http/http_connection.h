#ifndef __LYON__HTTP_CONNECTION_H__
#define __LYON__HTTP_CONNECTION_H__
#include "http_protocol.h"
#include "lyon/mutex.h"
#include "lyon/streams/socket_stream.h"
#include "lyon/uri.h"
#include <atomic>
#include <cstdint>
#include <list>
#include <lyon/util.h>
#include <memory>
namespace lyon::http {

struct HttpResult {
    typedef std::shared_ptr<HttpResult> ptr;
    enum class Error {
        //正常
        OK = 0,
        PARSER_URI_FAIL,
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
    HttpConnection(Socket::ptr sock);

    ~HttpConnection();

    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr request);

    static HttpResult::ptr DoRequest(HttpRequest::ptr request, Uri::ptr uri,
                                     uint64_t timeout_ms);

    static HttpResult::ptr
    DoRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout_ms,
              const std::map<std::string, std::string> &headers = {},
              const std::string &body = "");

    static HttpResult::ptr
    DoGet(Uri::ptr uri, uint64_t timeout_ms,
          const std::map<std::string, std::string> &headers = {},
          const std::string &body = "");

    static HttpResult::ptr
    DoGet(const std::string &uristr, uint64_t timeout_ms,
          const std::map<std::string, std::string> &headers = {},
          const std::string &body = "");

    static HttpResult::ptr
    DoPost(Uri::ptr uri, uint64_t timeout_ms,
           const std::map<std::string, std::string> &headers = {},
           const std::string &body = "");

    static HttpResult::ptr
    DoPost(const std::string &uristr, uint64_t timeout_ms,
           const std::map<std::string, std::string> &headers = {},
           const std::string &body = "");

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

    static HttpConnectionPool::ptr
    Create(const std::string &host, const std::string &vhost, uint16_t port,
           uint32_t maxSize, uint32_t maxLiveTime, uint32_t maxRequest);

    HttpConnection::ptr getConnection();

    HttpResult::ptr
    doGet(const std::string &path, uint64_t timeout_ms,
          const std::map<std::string, std::string> &headers = {},
          const std::string &body = "");

    HttpResult::ptr
    doGet(Uri::ptr uri, uint64_t timeout_ms,
          const std::map<std::string, std::string> &headers = {},
          const std::string &body = "");

    HttpResult::ptr
    doPost(const std::string &path, uint64_t timeout_ms,
           const std::map<std::string, std::string> &headers = {},
           const std::string &body = "");

    HttpResult::ptr
    doPost(Uri::ptr uri, uint64_t timeout_ms,
           const std::map<std::string, std::string> &headers = {},
           const std::string &body = "");

    HttpResult::ptr
    doRequest(HttpMethod method, const std::string &path, uint64_t timeout_ms,
              const std::map<std::string, std::string> &headers = {},
              const std::string &body = "");

    HttpResult::ptr doRequest(HttpRequest::ptr request, uint64_t timeout_ms);

private:
    static void ReleasePtr(HttpConnection *ptr, HttpConnectionPool *poll);

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

    /**
     * @m_connections 连接池
     */
    std::list<HttpConnection *> m_connections;

    /**
     * @m_total 当前的总连接数
     */
    std::atomic<uint32_t> m_total = {0};

    /**
     * @m_mutex 互斥量
     */
    MutexType m_mutex;
};

} // namespace lyon::http

#endif
