#ifndef __LYON_HTTP_PROTOCOL_H__
#define __LYON_HTTP_PROTOCOL_H__

#include <boost/lexical_cast.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace lyon::http {

/* Status Codes */
#define HTTP_STATUS_MAP(XX)                                                    \
    XX(100, CONTINUE, Continue)                                                \
    XX(101, SWITCHING_PROTOCOLS, Switching Protocols)                          \
    XX(102, PROCESSING, Processing)                                            \
    XX(200, OK, OK)                                                            \
    XX(201, CREATED, Created)                                                  \
    XX(202, ACCEPTED, Accepted)                                                \
    XX(203, NON_AUTHORITATIVE_INFORMATION, Non - Authoritative Information)    \
    XX(204, NO_CONTENT, No Content)                                            \
    XX(205, RESET_CONTENT, Reset Content)                                      \
    XX(206, PARTIAL_CONTENT, Partial Content)                                  \
    XX(207, MULTI_STATUS, Multi - Status)                                      \
    XX(208, ALREADY_REPORTED, Already Reported)                                \
    XX(226, IM_USED, IM Used)                                                  \
    XX(300, MULTIPLE_CHOICES, Multiple Choices)                                \
    XX(301, MOVED_PERMANENTLY, Moved Permanently)                              \
    XX(302, FOUND, Found)                                                      \
    XX(303, SEE_OTHER, See Other)                                              \
    XX(304, NOT_MODIFIED, Not Modified)                                        \
    XX(305, USE_PROXY, Use Proxy)                                              \
    XX(307, TEMPORARY_REDIRECT, Temporary Redirect)                            \
    XX(308, PERMANENT_REDIRECT, Permanent Redirect)                            \
    XX(400, BAD_REQUEST, Bad Request)                                          \
    XX(401, UNAUTHORIZED, Unauthorized)                                        \
    XX(402, PAYMENT_REQUIRED, Payment Required)                                \
    XX(403, FORBIDDEN, Forbidden)                                              \
    XX(404, NOT_FOUND, Not Found)                                              \
    XX(405, METHOD_NOT_ALLOWED, Method Not Allowed)                            \
    XX(406, NOT_ACCEPTABLE, Not Acceptable)                                    \
    XX(407, PROXY_AUTHENTICATION_REQUIRED, Proxy Authentication Required)      \
    XX(408, REQUEST_TIMEOUT, Request Timeout)                                  \
    XX(409, CONFLICT, Conflict)                                                \
    XX(410, GONE, Gone)                                                        \
    XX(411, LENGTH_REQUIRED, Length Required)                                  \
    XX(412, PRECONDITION_FAILED, Precondition Failed)                          \
    XX(413, PAYLOAD_TOO_LARGE, Payload Too Large)                              \
    XX(414, URI_TOO_LONG, URI Too Long)                                        \
    XX(415, UNSUPPORTED_MEDIA_TYPE, Unsupported Media Type)                    \
    XX(416, RANGE_NOT_SATISFIABLE, Range Not Satisfiable)                      \
    XX(417, EXPECTATION_FAILED, Expectation Failed)                            \
    XX(421, MISDIRECTED_REQUEST, Misdirected Request)                          \
    XX(422, UNPROCESSABLE_ENTITY, Unprocessable Entity)                        \
    XX(423, LOCKED, Locked)                                                    \
    XX(424, FAILED_DEPENDENCY, Failed Dependency)                              \
    XX(426, UPGRADE_REQUIRED, Upgrade Required)                                \
    XX(428, PRECONDITION_REQUIRED, Precondition Required)                      \
    XX(429, TOO_MANY_REQUESTS, Too Many Requests)                              \
    XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large)  \
    XX(451, UNAVAILABLE_FOR_LEGAL_REASONS, Unavailable For Legal Reasons)      \
    XX(500, INTERNAL_SERVER_ERROR, Internal Server Error)                      \
    XX(501, NOT_IMPLEMENTED, Not Implemented)                                  \
    XX(502, BAD_GATEWAY, Bad Gateway)                                          \
    XX(503, SERVICE_UNAVAILABLE, Service Unavailable)                          \
    XX(504, GATEWAY_TIMEOUT, Gateway Timeout)                                  \
    XX(505, HTTP_VERSION_NOT_SUPPORTED, HTTP Version Not Supported)            \
    XX(506, VARIANT_ALSO_NEGOTIATES, Variant Also Negotiates)                  \
    XX(507, INSUFFICIENT_STORAGE, Insufficient Storage)                        \
    XX(508, LOOP_DETECTED, Loop Detected)                                      \
    XX(510, NOT_EXTENDED, Not Extended)                                        \
    XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required)

enum HttpStatus {
#define XX(num, name, string) name = num,
    HTTP_STATUS_MAP(XX)
#undef XX
        HTTP_STATUS_INVALID = 0
};

const char *HttpStatus2String(HttpStatus status);

/* Request Methods */
#define HTTP_METHOD_MAP(XX)                                                    \
    XX(0, DELETE, DELETE)                                                      \
    XX(1, GET, GET)                                                            \
    XX(2, HEAD, HEAD)                                                          \
    XX(3, POST, POST)                                                          \
    XX(4, PUT, PUT)                                                            \
    /* pathological */                                                         \
    XX(5, CONNECT, CONNECT)                                                    \
    XX(6, OPTIONS, OPTIONS)                                                    \
    XX(7, TRACE, TRACE)                                                        \
    /* WebDAV */                                                               \
    XX(8, COPY, COPY)                                                          \
    XX(9, LOCK, LOCK)                                                          \
    XX(10, MKCOL, MKCOL)                                                       \
    XX(11, MOVE, MOVE)                                                         \
    XX(12, PROPFIND, PROPFIND)                                                 \
    XX(13, PROPPATCH, PROPPATCH)                                               \
    XX(14, SEARCH, SEARCH)                                                     \
    XX(15, UNLOCK, UNLOCK)                                                     \
    XX(16, BIND, BIND)                                                         \
    XX(17, REBIND, REBIND)                                                     \
    XX(18, UNBIND, UNBIND)                                                     \
    XX(19, ACL, ACL)                                                           \
    /* subversion */                                                           \
    XX(20, REPORT, REPORT)                                                     \
    XX(21, MKACTIVITY, MKACTIVITY)                                             \
    XX(22, CHECKOUT, CHECKOUT)                                                 \
    XX(23, MERGE, MERGE)                                                       \
    /* upnp */                                                                 \
    XX(24, MSEARCH, M - SEARCH)                                                \
    XX(25, NOTIFY, NOTIFY)                                                     \
    XX(26, SUBSCRIBE, SUBSCRIBE)                                               \
    XX(27, UNSUBSCRIBE, UNSUBSCRIBE)                                           \
    /* RFC-5789 */                                                             \
    XX(28, PATCH, PATCH)                                                       \
    XX(29, PURGE, PURGE)                                                       \
    /* CalDAV */                                                               \
    XX(30, MKCALENDAR, MKCALENDAR)                                             \
    /* RFC-2068, section 19.6.1.2 */                                           \
    XX(31, LINK, LINK)                                                         \
    XX(32, UNLINK, UNLINK)                                                     \
    /* icecast */                                                              \
    XX(33, SOURCE, SOURCE)

enum HttpMethod {
#define XX(num, name, string) name = num,
    HTTP_METHOD_MAP(XX)
#undef XX
        HTTP_METHOD_INVALID = 100
};

const char *HttpMethod2String(HttpMethod method);
HttpMethod String2HttpMethod(const char *str);
HttpMethod String2HttpMethod(const std::string &str);

struct CaseInsensitiveLess {
    bool operator()(const std::string &lhs, const std::string &rhs) const;
};

template <typename MapType, typename T>
T getAs(const MapType &m, const std::string &key, const T &def = T()) {
    auto itr = m.find(key);
    if (itr == m.end()) {
        return def;
    }
    try {
        return boost::lexical_cast<T>(itr->second);
    } catch (...) {
    }
    return def;
}

/**
 * @brief 请求协议对应的解析类
 */
class HttpRequest {
public:
    typedef std::shared_ptr<HttpRequest> ptr;
    typedef std::map<std::string, std::string, CaseInsensitiveLess> MyMap;

    HttpRequest(uint8_t version = 0x11, bool connection = false);

    /**
     * @brief 获取完整的协议头
     *
     * @return 协议头
     */
    const MyMap &getHeaders() const { return m_headers; }
    /**
     * @brief 获取HTTP请求方法
     *
     * @return HTTP请求方法
     */
    const HttpMethod getMethod() const { return m_method; }
    /**
     * @brief 获取请求路径
     *
     */
    const std::string &getPath() const { return m_path; }
    /**
     * @brief 获取HTTP协议版本
     *
     * @return HTTP协议版本。0x11表示HTTP1.1
     */
    const uint8_t getVersion() const { return m_version; }
    /**
     * @brief 是否保持长连接
     *
     */
    const bool isConnection() const { return m_connection; }
    /**
     * @brief 获取所有Cookies
     *
     */
    const MyMap &getCookies() const { return m_cookies; }
    /**
     * @brief 获取请求参数
     *
     */
    const MyMap &getParams() const { return m_params; }
    /**
     * @brief 获取请求体
     *
     */
    const std::string &getBody() const { return m_body; }

    const std::string &getHeader(const std::string &key,
                                 const std::string &def = "") const;

    const std::string &getCookie(const std::string &key,
                                 const std::string &def = "") const;

    const std::string &getParam(const std::string &key,
                                const std::string &def = "") const;

    void setMethod(const HttpMethod v) { m_method = v; }
    void setPath(const std::string &v) { m_path = v; }
    void setVersion(uint8_t v) { m_version = v; }
    void setConnection(bool v) { m_connection = v; }
    void setBody(const std::string &v) { m_body = v; }
    void setQuery(const std::string &v) { m_query = v; }
    void setFragment(const std::string &v) { m_fragment = v; }

    void setHeader(const std::string &key, const std::string &val);
    void setCookie(const std::string &key, const std::string &val);
    void setParam(const std::string &key, const std::string &val);

    bool delHeader(const std::string &key);
    bool delCookie(const std::string &key);
    bool delParam(const std::string &key);

    std::ostream &dump(std::ostream &os) const;
    std::string toString() const;

public:
    template <typename T>
    T getHeaderAs(const std::string &key, const T &def = T()) {
        return getAs(m_headers, key, def);
    }

    template <typename T>
    T getCookisAs(const std::string &key, const T &def = T()) {
        return getAs(m_cookies, key, def);
    }

    template <typename T>
    T getParamAs(const std::string &key, const T &def = T()) {
        return getAs(m_params, key, def);
    }

private:
    /**
     * @m_headers 请求头
     */
    MyMap m_headers;
    /**
     * @m_method 请求方法
     */
    HttpMethod m_method;
    /**
     * @m_path 请求路径
     */
    std::string m_path;
    /**
     * @m_version Http版本
     */
    uint8_t m_version;
    /**
     * @m_connection 是否保持长连接
     */
    bool m_connection;
    /**
     * @m_cookies Cookies
     */
    MyMap m_cookies;
    /**
     * @m_params 请求参数
     */
    MyMap m_params;
    /**
     * @m_body 请求体
     */
    std::string m_body;
    /**
     * @m_query 请求参数
     */
    std::string m_query;
    /**
     * @m_fragment 请求fragment
     */
    std::string m_fragment;

    bool m_websocket;
};

std::ostream &operator<<(std::ostream &os, const HttpRequest &request);

/**
 * @brief 响应协议对应的解析类
 */
class HttpResponse {
public:
    typedef std::shared_ptr<HttpResponse> ptr;
    typedef std::map<std::string, std::string, CaseInsensitiveLess> MyMap;

    HttpResponse(uint8_t version = 0x11, bool connection = true);

    const MyMap &getHeaders() const { return m_headers; }
    const uint8_t getVersion() const { return m_version; }
    const HttpStatus getStatus() const { return m_status; }
    const bool isConnection() const { return m_connection; }
    const std::vector<std::string> &getCookies() const { return m_cookies; }
    const std::string &getBody() const { return m_body; }

    const std::string &getHeader(const std::string &key,
                                 const std::string &def = "");

    void setVersion(uint8_t v) { m_version = v; }
    void setHttpStatus(HttpStatus v) { m_status = v; }
    void setConnection(bool v) { m_connection = v; }
    void setBody(const std::string &v) { m_body = v; }
    void setReason(const std::string &v) { m_reason = v; }

    void setHeader(const std::string &key, const std::string &val);
    void addCookie(const std::string &v);

    std::ostream &dump(std::ostream &os) const;
    std::string toString() const;

public:
    template <typename T>
    T getHeaderAs(const std::string &key, const T &def = T()) {
        return getAs(m_headers, key, def);
    }

private:
    MyMap m_headers;
    uint8_t m_version;
    HttpStatus m_status;
    bool m_connection;
    std::vector<std::string> m_cookies;
    std::string m_body;
    std::string m_reason;

    bool m_websocket;
};

std::ostream &operator<<(std::ostream &os, const HttpResponse &response);
} // namespace lyon::http

#endif
