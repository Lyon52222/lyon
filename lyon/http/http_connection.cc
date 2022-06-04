#include "http_connection.h"
#include "http_parser.h"
#include "http_protocol.h"
#include "lyon/log.h"
#include <cstdint>
#include <lyon/address.h>
#include <lyon/socket.h>
#include <lyon/util.h>
#include <memory>
#include <sstream>
namespace lyon {
namespace http {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

std::string HttpResult::toString() const {
    std::stringstream ss;
    ss << "HttpResult: result = " << (int)result << " error = " << error;
    if (response)
        ss << " Response = " << response->toString();
    return ss.str();
}

HttpResponse::ptr HttpConnection::recvResponse() {
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
        buf[readed] = '\0';
        size_t excuted = response_parser->excute(buf, readed, 0);
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
            if (content_length > 0) {
                if (readFixSize(&body[readed], content_length) <= 0) {
                    close();
                    return nullptr;
                }
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

                buf[readed] = '\0';
                size_t executed = response_parser->excute(buf, readed, 0, true);
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

int HttpConnection::sendRequest(HttpRequest::ptr request) {
    std::string str = request->toString();
    return writeFixSize(str.c_str(), str.size());
}

HttpResult::ptr HttpConnection::DoRequest(HttpRequest::ptr request,
                                          Uri::ptr uri, uint64_t timeout_ms) {

    //通过uri创建地址
    Address::ptr addr = uri->createIPAddress();
    if (!addr) {
        return std::make_shared<HttpResult>(HttpResult::Error::INVALID_HOST,
                                            nullptr,
                                            "invalid host" + uri->getHost());
    }
    //通过地址建立socket连接
    Socket::ptr sock = Socket::CreateTCP(addr);
    if (!sock) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::CREATE_SOCKET_ERROR, nullptr,
            "create socket error" + addr->toString());
    }

    //连接到server
    if (!sock->connect(addr)) {
        return std::make_shared<HttpResult>(HttpResult::Error::CONNECT_FAIL,
                                            nullptr,
                                            "connect fail" + sock->toString());
    }
    //设置sock接收超时时间
    sock->setRecvTimeout(timeout_ms);

    //通过socket连接封装connection
    HttpConnection::ptr conn = std::make_shared<HttpConnection>(sock);

    if (!conn) {
        return std::make_shared<HttpResult>(HttpResult::Error::CONNECT_FAIL,
                                            nullptr,
                                            "connect fail" + sock->toString());
    }

    //向connection发送请求
    int rt = conn->sendRequest(request);
    if (rt == 0) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::SEND_CLOSE_BY_PEER, nullptr,
            "send close by peer" + sock->toString());
    } else if (rt < 0) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::SEND_SOCKET_ERROR, nullptr,
            "send socket error" + sock->toString());
    }

    //从connection接收响应
    HttpResponse::ptr response = conn->recvResponse();
    if (!response) {
        return std::make_shared<HttpResult>(HttpResult::Error::TIMEOUT, nullptr,
                                            "recv timeout" + sock->toString());
    }

    return std::make_shared<HttpResult>(HttpResult::Error::OK, response, "OK");
}

HttpResult::ptr
HttpConnection::DoRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout_ms,
                          const std::map<std::string, std::string> &headers,
                          const std::string &body) {
    HttpRequest::ptr request(new HttpRequest());
    request->setMethod(method);
    request->setPath(uri->getPath());
    request->setQuery(uri->getQuery());
    request->setFragment(uri->getFragment());
    bool has_host = false;
    for (auto &header : headers) {
        if (strcasecmp(header.first.c_str(), "connection") == 0) {
            if (strcasecmp(header.second.c_str(), "keep-alive")) {
                request->setConnection(true);
            } else {
                request->setConnection(false);
            }
        }
        if (strcasecmp(header.first.c_str(), "host")) {
            if (!header.second.empty()) {
                has_host = true;
            } else {
                continue;
            }
        }
        request->setHeader(header.first, header.second);
    }
    if (!has_host) {
        request->setHeader("host", uri->getHost());
    }
    request->setBody(body);

    return DoRequest(request, uri, timeout_ms);
}

HttpResult::ptr
HttpConnection::DoGet(Uri::ptr uri, uint64_t timeout_ms,
                      const std::map<std::string, std::string> &headers,
                      const std::string &body) {
    return DoRequest(HttpMethod::GET, uri, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnection::DoGet(const std::string &uristr, uint64_t timeout_ms,
                      const std::map<std::string, std::string> &headers,
                      const std::string &body) {
    Uri::ptr uri = Uri::Parser(uristr);
    if (!uri) {
        return std::make_shared<HttpResult>(HttpResult::Error::PARSER_URI_FAIL,
                                            nullptr, "parser uri fail");
    }
    return DoGet(uri, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnection::DoPost(Uri::ptr uri, uint64_t timeout_ms,
                       const std::map<std::string, std::string> &headers,
                       const std::string &body) {
    return DoRequest(HttpMethod::POST, uri, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnection::DoPost(const std::string &uristr, uint64_t timeout_ms,
                       const std::map<std::string, std::string> &headers,
                       const std::string &body) {
    Uri::ptr uri = Uri::Parser(uristr);
    if (!uri) {
        return std::make_shared<HttpResult>(HttpResult::Error::PARSER_URI_FAIL,
                                            nullptr, "parser uri fail");
    }
    return DoPost(uri, timeout_ms, headers, body);
}

HttpConnectionPool::HttpConnectionPool(const std::string &host,
                                       const std::string &vhost, uint16_t port,
                                       uint32_t maxSize, uint32_t maxLiveTime,
                                       uint32_t maxRequest)
    : m_host(host), m_vhost(vhost), m_port(port), m_maxSize(maxSize),
      m_maxLiveTime(maxLiveTime), m_maxRequest(maxRequest) {}

HttpConnectionPool::ptr
HttpConnectionPool::Create(const std::string &host, const std::string &vhost,
                           uint16_t port, uint32_t maxSize,
                           uint32_t maxLiveTime, uint32_t maxRequest) {
    return std::make_shared<HttpConnectionPool>(host, vhost, port, maxSize,
                                                maxLiveTime, maxRequest);
}

HttpConnection::ptr HttpConnectionPool::getConnection() {
    std::vector<HttpConnection *> invalid_connections;
    HttpConnection *rt = nullptr;

    MutexType::Lock lock(m_mutex);

    //从连接池中找到第一个有效连接，并且将无效连接删除掉。
    while (!m_connections.empty()) {
        auto conn = m_connections.front();
        m_connections.pop_front();

        if (!conn->isConnected()) {
            invalid_connections.push_back(conn);
            continue;
        }

        if (conn->getCreateTime() + m_maxLiveTime >= GetCurrentTimeMS()) {
            invalid_connections.push_back(conn);
            continue;
        }
        rt = conn;
        break;
    }

    lock.unlock();
    for (auto &conn : invalid_connections) {
        delete conn;
    }
    m_total -= invalid_connections.size();
    //如果找不到有效连接的话就创建一个
    if (!rt) {
        IPAddress::ptr addr = Address::LookUpAnyIpAddress(m_host);
        if (!addr) {
            return nullptr;
        }

        addr->setPort(m_port);

        Socket::ptr sock = Socket::CreateTCP(addr);
        if (!sock) {
            return nullptr;
        }

        if (!sock->connect(addr)) {
            return nullptr;
        }

        rt = new HttpConnection(sock);

        ++m_total;
    }

    //这里使用shared_ptr进行封装，使得。Connection在使用完后，并不是直接释放掉。而是将连接加入到连接池中。
    return HttpConnection::ptr(rt, std::bind(HttpConnectionPool::ReleasePtr,
                                             std::placeholders::_1, this));
}

HttpResult::ptr HttpConnectionPool::doRequest(HttpRequest::ptr request,
                                              uint64_t timeout_ms) {
    auto conn = getConnection();
    if (!conn) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::POOL_GET_CONN_FAIL, nullptr,
            "Pool get connection fail");
    }

    Socket::ptr sock = conn->getSocket();
    if (!sock) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::POOL_INVALID_SOCK, nullptr, "Pool invalid sock");
    }

    sock->setRecvTimeout(timeout_ms);

    int rt = conn->sendRequest(request);
    if (rt == 0) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::SEND_CLOSE_BY_PEER, nullptr,
            "send close by peer" + sock->toString());
    } else if (rt < 0) {
        return std::make_shared<HttpResult>(
            HttpResult::Error::SEND_SOCKET_ERROR, nullptr,
            "send socket error" + sock->toString());
    }

    HttpResponse::ptr response = conn->recvResponse();
    if (!response) {
        return std::make_shared<HttpResult>(HttpResult::Error::TIMEOUT, nullptr,
                                            "recv timeout" + sock->toString());
    }

    return std::make_shared<HttpResult>(HttpResult::Error::OK, response, "OK");
}

void HttpConnectionPool::ReleasePtr(HttpConnection *ptr,
                                    HttpConnectionPool *pool) {
    //这里将socket连接进行重用
    ptr->incRequest();
    if (!ptr->isConnected() ||
        (ptr->getCreateTime() + pool->m_maxLiveTime >= GetCurrentTimeMS()) ||
        ptr->getRequest() > pool->m_maxRequest ||
        pool->m_total > pool->m_maxSize) {
        delete ptr;
        --pool->m_total;
        return;
    }
    MutexType::Lock lock(pool->m_mutex);
    pool->m_connections.push_back(ptr);
}

HttpResult::ptr
HttpConnectionPool::doGet(const std::string &path, uint64_t timeout_ms,
                          const std::map<std::string, std::string> &headers,
                          const std::string &body) {
    return doRequest(HttpMethod::GET, path, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doGet(Uri::ptr uri, uint64_t timeout_ms,
                          const std::map<std::string, std::string> &headers,
                          const std::string &body) {
    std::stringstream ss;
    ss << uri->getPath() << (uri->getQuery().empty() ? "" : "?")
       << uri->getQuery() << (uri->getFragment().empty() ? "" : "#")
       << uri->getFragment();
    std::string path = ss.str();
    return doRequest(HttpMethod::GET, path, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doPost(const std::string &path, uint64_t timeout_ms,
                           const std::map<std::string, std::string> &headers,
                           const std::string &body) {
    return doRequest(HttpMethod::POST, path, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doPost(Uri::ptr uri, uint64_t timeout_ms,
                           const std::map<std::string, std::string> &headers,
                           const std::string &body) {
    std::stringstream ss;
    ss << uri->getPath() << (uri->getQuery().empty() ? "" : "?")
       << uri->getQuery() << (uri->getFragment().empty() ? "" : "#")
       << uri->getFragment();
    std::string path = ss.str();
    return doRequest(HttpMethod::POST, path, timeout_ms, headers, body);
}

HttpResult::ptr
HttpConnectionPool::doRequest(HttpMethod method, const std::string &path,
                              uint64_t timeout_ms,
                              const std::map<std::string, std::string> &headers,
                              const std::string &body) {
    HttpRequest::ptr request(new HttpRequest());
    request->setMethod(method);
    request->setPath(path);
    request->setConnection(true);
    bool has_host = false;
    for (auto &header : headers) {
        if (strcasecmp(header.first.c_str(), "connection") == 0) {
            if (strcasecmp(header.second.c_str(), "keep-alive") == 0) {
                request->setConnection(true);
            } else {
                request->setConnection(false);
            }
        }
        if (strcasecmp(header.first.c_str(), "host")) {
            if (!header.second.empty()) {
                has_host = true;
            } else {
                continue;
            }
        }
        request->setHeader(header.first, header.second);
    }
    if (!has_host) {
        if (!m_host.empty())
            request->setHeader("Host", m_host);
        else
            request->setHeader("Host", m_vhost);
    }
    request->setBody(body);
    return doRequest(request, timeout_ms);
}
} // namespace http
} // namespace lyon
