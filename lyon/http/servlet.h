#ifndef __LYON_HTTP_SERVLET_H__
#define __LYON_HTTP_SERVLET_H__
#include <functional>
#include <http_protocol.h>
#include <http_session.h>
#include <memory>
#include <unordered_map>

namespace lyon {
namespace http {

/**
 * @brief Servlet虚基类，用于处理request的请求,并将结果保存到response中
 */
class Servlet {
public:
    typedef std::shared_ptr<Servlet> ptr;

    Servlet(const std::string &name) : m_name(name) {}

    virtual int32_t handle(const HttpRequest::ptr request,
                           HttpResponse::ptr response,
                           HttpSession::ptr session) = 0;

    virtual ~Servlet() = default;

private:
    std::string m_name;
};

class FunctionalServlet : public Servlet {
public:
    typedef std::shared_ptr<FunctionalServlet> ptr;
    typedef std::function<int32_t(const HttpRequest::ptr, HttpResponse::ptr,
                                  HttpSession::ptr)>
        callback;

    FunctionalServlet(const std::string &name, callback cb)
        : Servlet(name), m_cb(cb) {}

    virtual int32_t handle(const HttpRequest::ptr request,
                           HttpResponse::ptr response,
                           HttpSession::ptr session) override;

private:
    callback m_cb;
};

/**
 * @brief 负责Servlet的注册与派发
 */
class ServletDispatch : public Servlet {
public:
    typedef std::shared_ptr<ServletDispatch> ptr;

    ServletDispatch();

    virtual int32_t handle(const HttpRequest::ptr request,
                           HttpResponse::ptr response,
                           HttpSession::ptr session) override;

    void addServlet(const std::string &url, const Servlet::ptr servlet) const;

    bool deleServlet(const std::string &url) const;

    const Servlet::ptr getServlet(const std::string &url) const;

private:
    std::unordered_map<std::string, Servlet::ptr> m_servlets;
};

} // namespace http

} // namespace lyon

#endif
