#include "http.h"
#include <cstdint>
#include <sstream>
namespace lyon {
namespace http {

const char *HttpStatus2String(HttpStatus status) {
    switch (status) {
#define XX(num, name, string)                                                  \
    case HttpStatus::name:                                                     \
        return #string;                                                        \
        break;

        HTTP_STATUS_MAP(XX)
#undef XX
    default:
        return "invalid status";
    }
}

const char *HttpMethod2String(HttpMethod method) {
    switch (method) {
#define XX(num, name, string)                                                  \
    case HttpMethod::name:                                                     \
        return #string;
        break;
        HTTP_METHOD_MAP(XX)
#undef XX
    default:
        return "invalid method";
    }
}

HttpMethod String2HttpMethod(const char *str) {

    return HttpMethod::HTTP_METHOD_INVALID;
}
HttpMethod String2HttpMethod(const std::string &str) {

    return HttpMethod::HTTP_METHOD_INVALID;
}

bool CaseInsensitiveLess::operator()(const std::string &lhs,
                                     const std::string &rhs) const {
    return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
}

HttpRequest::HttpRequest(uint8_t version, bool connection)
    : m_method(HttpMethod::GET), m_path("/"), m_version(version),
      m_connection(connection), m_websocket(false) {}

const std::string &HttpRequest::getHeader(const std::string &key,
                                          const std::string &def) const {
    auto itr = m_headers.find(key);
    if (itr == m_headers.end()) {
        return def;
    }
    return itr->second;
}

const std::string &HttpRequest::getCookie(const std::string &key,
                                          const std::string &def) const {
    auto itr = m_cookies.find(key);
    if (itr == m_cookies.end()) {
        return def;
    }
    return itr->second;
}

const std::string &HttpRequest::getParam(const std::string &key,
                                         const std::string &def) const {
    auto itr = m_params.find(key);
    if (itr == m_params.end()) {
        return def;
    }
    return itr->second;
}

void HttpRequest::setHeader(const std::string &key, const std::string &val) {
    m_headers[key] = val;
}

void HttpRequest::setCookie(const std::string &key, const std::string &val) {
    m_cookies[key] = val;
}

void HttpRequest::setParam(const std::string &key, const std::string &val) {
    m_params[key] = val;
}

bool HttpRequest::delHeader(const std::string &key) {
    return m_headers.erase(key);
}

bool HttpRequest::delCookie(const std::string &key) {
    return m_cookies.erase(key);
}

bool HttpRequest::delParam(const std::string &key) {
    return m_params.erase(key);
}

std::ostream &HttpRequest::dump(std::ostream &os) const {
    os << HttpMethod2String(m_method) << " " << m_path << " HTTP"
       << (m_version >> 4) << "." << (m_version & 0x0f) << "\r\n";
    if (!m_websocket) {
        os << "Connection: " << (m_connection ? "keep-alive" : "close")
           << "\r\n";
    }
    for (auto &i : m_headers) {
        os << i.first << ": " << i.second << "\r\n";
    }

    if (!m_body.empty()) {
        os << "content-length: " << m_body.size() << "\r\n\r\n" << m_body;
    } else {
        os << "\r\n";
    }

    return os;
}

std::string HttpRequest::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const HttpRequest &request) {
    return request.dump(os);
}

HttpResponse::HttpResponse(uint8_t version, bool connection)
    : m_version(version), m_status(HttpStatus::OK), m_connection(connection),
      m_websocket(false) {}

const std::string &HttpResponse::getHeader(const std::string &key,
                                           const std::string &def) {
    auto itr = m_headers.find(key);
    if (itr == m_headers.end()) {
        return def;
    }
    return itr->second;
}

void HttpResponse::addCookie(const std::string &v) { m_cookies.push_back(v); }

std::ostream &HttpResponse::dump(std::ostream &os) const {
    os << "HTTP/" << (m_version >> 4) << "." << (m_version & 0x0f) << " "
       << (uint32_t)m_status << " " << HttpStatus2String(m_status) << "\r\n";
    if (!m_websocket) {
        os << "Connection: " << (m_connection ? "keep-alive" : "close")
           << "\r\n";
    }

    for (auto &i : m_headers) {
        os << i.first << ": " << i.second << "\r\n";
    }

    for (auto &i : m_cookies) {
        os << "Set-Cookie: " << i << "\r\n";
    }

    if (!m_body.empty()) {
        os << "content-length: " << m_body.size() << "\r\n\r\n" << m_body;
    } else {
        os << "\r\n";
    }

    return os;
}

std::string HttpResponse ::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

std::ostream &operator<<(std::ostream &os, const HttpResponse &response) {
    return response.dump(os);
}
} // namespace http
} // namespace lyon
