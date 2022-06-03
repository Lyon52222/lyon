#ifndef __LYON_HTTP_HTTP_SERVLET_H__
#define __LYON_HTTP_HTTP_SERVLET_H__
#include "http_protocol.h"
#include "http_session.h"
#include "lyon/mutex.h"
#include <functional>
#include <list>
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

    virtual int32_t handle(HttpRequest::ptr request, HttpResponse::ptr response,
                           HttpSession::ptr session) = 0;

    virtual Servlet::ptr clone() const;

    virtual ~Servlet() = default;

private:
    std::string m_name;
};

/**
 * @brief 函数Servlet,使用函数处理servlet.
 */
class FunctionalServlet : public Servlet {
public:
    typedef std::shared_ptr<FunctionalServlet> ptr;
    typedef std::function<int32_t(HttpRequest::ptr, HttpResponse::ptr,
                                  HttpSession::ptr)>
        callback;

    FunctionalServlet(callback cb,
                      const std::string &name = "FunctionalServlet")
        : Servlet(name), m_cb(cb) {}

    virtual int32_t handle(HttpRequest::ptr request, HttpResponse::ptr response,
                           HttpSession::ptr session) override;

    virtual Servlet::ptr clone() const override;

private:
    callback m_cb;
};

class IServletCreator {
public:
    typedef std::shared_ptr<IServletCreator> ptr;
    virtual Servlet::ptr create() const = 0;
};

/**
 * @brief 每次返回的是都相同的Servlet
 */
class HoldServletCreator : public IServletCreator {
public:
    typedef std::shared_ptr<HoldServletCreator> ptr;

    HoldServletCreator(Servlet::ptr hold) : m_hold(hold) {}
    virtual Servlet::ptr create() const override { return m_hold; }

private:
    Servlet::ptr m_hold;
};

/**
 * @brief 调用clone构造的一个相同的servlet
 */
class ProtoServletCreator : public IServletCreator {
public:
    typedef std::shared_ptr<ProtoServletCreator> ptr;

    ProtoServletCreator(Servlet::ptr proto) : m_proto(proto) {}

    virtual Servlet::ptr create() const override { return m_proto->clone(); }

private:
    Servlet::ptr m_proto;
};

/**
 * @brief 创建一个新的Servlet
 *
 * @tparam T Servlet类型
 */
template <class T> class NewServletCreator : public IServletCreator {
public:
    typedef std::shared_ptr<NewServletCreator<T>> ptr;
    virtual Servlet::ptr create() const override { return Servlet::ptr(new T); }
};

/**
 * @brief 负责Servlet的注册与派发
 */
class ServletDispatch : public Servlet {
public:
    typedef std::shared_ptr<ServletDispatch> ptr;
    typedef RWMutex RWMutexType;

    ServletDispatch();

    virtual int32_t handle(HttpRequest::ptr request, HttpResponse::ptr response,
                           HttpSession::ptr session) override;

    void addAccurateServlet(const std::string &url,
                            FunctionalServlet::callback cb);

    void addAccurateServlet(const std::string &url, Servlet::ptr servlet);

    void addAccurateServlet(const std::string &url,
                            IServletCreator::ptr servlet_creator);

    bool deleAccurateServlet(const std::string &url);

    void addFuzzyServlet(const std::string &url, Servlet::ptr servlet);

    void addFuzzyServlet(const std::string &url,
                         IServletCreator::ptr servlet_creator);

    bool deleFuzzyServlet(const std::string &url);

    /**
     * @brief 获取精确匹配的Servlet
     *
     * @param url 需要精确匹配的url
     */
    Servlet::ptr getAccurateServlet(const std::string &url);

    /**
     * @brief 获取模糊匹配的Servlet
     *
     * @param url 需要模糊匹配的url
     */
    Servlet::ptr getFuzzyServlet(const std::string &url);

    /**
     * @brief 获取Servlet 优先精确匹配，其次模糊匹配
     *
     * @param url 需要匹配的url
     */
    Servlet::ptr getServlet(const std::string &url);

    void setDefaultServlet(Servlet::ptr servlet) { m_defaultServlet = servlet; }

    Servlet::ptr getDefaultServlet() const { return m_defaultServlet; }

private:
    RWMutexType m_mutex;
    /**
     * @m_accurateServlets 只允许准确匹配的servlet
     */
    std::unordered_map<std::string, IServletCreator::ptr> m_accurateServlets;
    /**
     * @m_fuzzyServlets 只允许模糊匹配的servlet
     */
    std::list<std::pair<std::string, IServletCreator::ptr>> m_fuzzyServlets;
    Servlet::ptr m_defaultServlet;
};

class NotFoundServlet : public Servlet {
public:
    typedef std::shared_ptr<NotFoundServlet> ptr;

    NotFoundServlet(const std::string &name);

    virtual int32_t handle(HttpRequest::ptr request, HttpResponse::ptr response,
                           HttpSession::ptr session) override;

private:
    std::string m_content;
};

} // namespace http

} // namespace lyon

#endif
