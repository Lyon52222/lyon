#include "http_servlet.h"
#include "http_protocol.h"
#include <fnmatch.h>
#include <memory>
namespace lyon {
namespace http {

Servlet::ptr Servlet::clone() const { return nullptr; }

int32_t FunctionalServlet::handle(HttpRequest::ptr request,
                                  HttpResponse::ptr response,
                                  HttpSession::ptr session) {
    return m_cb(request, response, session);
}

Servlet::ptr FunctionalServlet::clone() const {
    Servlet::ptr copy = Servlet::ptr(new FunctionalServlet(*this));
    return copy;
}

ServletDispatch::ServletDispatch() : Servlet("Dispatch") {
    m_defaultServlet.reset(new NotFoundServlet("404 Not Found"));
}

int32_t ServletDispatch::handle(HttpRequest::ptr request,
                                HttpResponse::ptr response,
                                HttpSession::ptr session) {
    Servlet::ptr servlet = getAccurateServlet(request->getPath());
    if (servlet) {
        return servlet->handle(request, response, session);
    }
    return m_defaultServlet->handle(request, response, session);
}

void ServletDispatch::addAccurateServlet(const std::string &url,
                                         FunctionalServlet::callback cb) {
    FunctionalServlet::ptr f_servlet =
        FunctionalServlet::ptr(new FunctionalServlet(cb));
    addAccurateServlet(url, f_servlet);
}

void ServletDispatch::addAccurateServlet(const std::string &url,
                                         Servlet::ptr servlet) {
    RWMutexType::WRLock wlock(m_mutex);
    m_accurateServlets[url] = std::make_shared<HoldServletCreator>(servlet);
}

void ServletDispatch::addAccurateServlet(const std::string &url,
                                         IServletCreator::ptr servlet_creator) {
    RWMutexType::WRLock wlock(m_mutex);
    m_accurateServlets[url] = servlet_creator;
}

bool ServletDispatch::deleAccurateServlet(const std::string &url) {
    RWMutexType::WRLock wlock(m_mutex);
    auto itr = m_accurateServlets.find(url);
    if (itr == m_accurateServlets.end()) {
        return false;
    }
    return m_accurateServlets.erase(url);
}

void ServletDispatch::addFuzzyServlet(const std::string &url,
                                      Servlet::ptr servlet) {
    RWMutexType::WRLock wlock(m_mutex);
    auto itr = m_fuzzyServlets.begin();
    while (itr != m_fuzzyServlets.end()) {
        if (itr->first == url) {
            break;
        }
        ++itr;
    }
    if (itr == m_fuzzyServlets.end()) {
        m_fuzzyServlets.push_back(
            std::make_pair(url, std::make_shared<HoldServletCreator>(servlet)));
        return;
    }
    itr->second = std::make_shared<HoldServletCreator>(servlet);
}

void ServletDispatch::addFuzzyServlet(const std::string &url,
                                      IServletCreator::ptr servlet_creator) {
    RWMutexType::WRLock wlock(m_mutex);
    auto itr = m_fuzzyServlets.begin();
    while (itr != m_fuzzyServlets.end()) {
        if (itr->first == url) {
            break;
        }
        ++itr;
    }
    if (itr == m_fuzzyServlets.end()) {
        m_fuzzyServlets.push_back(std::make_pair(url, servlet_creator));
        return;
    }
    itr->second = servlet_creator;
}

bool ServletDispatch::deleFuzzyServlet(const std::string &url) {
    RWMutexType::WRLock wlock(m_mutex);
    for (auto itr = m_fuzzyServlets.begin(); itr != m_fuzzyServlets.end();
         ++itr) {
        if (itr->first == url) {
            m_fuzzyServlets.erase(itr);
            return true;
        }
    }
    return false;
}

Servlet::ptr ServletDispatch::getAccurateServlet(const std::string &url) {
    RWMutexType::RDLock rlock(m_mutex);
    auto itr = m_accurateServlets.find(url);
    if (itr == m_accurateServlets.end()) {
        return nullptr;
    }
    return itr->second->create();
}

Servlet::ptr ServletDispatch::getFuzzyServlet(const std::string &url) {
    RWMutexType::RDLock rlock(m_mutex);
    for (auto itr = m_fuzzyServlets.begin(); itr != m_fuzzyServlets.end();
         ++itr) {
        if (itr->first == url) {
            return itr->second->create();
        }
    }
    return nullptr;
}

Servlet::ptr ServletDispatch::getServlet(const std::string &url) {
    RWMutexType::RDLock rlock(m_mutex);
    auto itr = m_accurateServlets.find(url);
    if (itr != m_accurateServlets.end()) {
        return itr->second->create();
    }
    for (auto itr1 = m_fuzzyServlets.begin(); itr1 != m_fuzzyServlets.end();
         ++itr1) {
        if (!fnmatch(itr->first.c_str(), url.c_str(), 0)) {
            return itr->second->create();
        }
    }
    return nullptr;
}

NotFoundServlet::NotFoundServlet(const std::string &name) : Servlet(name) {
    m_content = "<html><head> <title>404 Not Found</title> </head><body> "
                "<h1>Not Found</h1> <p>The requested URL {URL} was not "
                "found on this server.</p> </body></html>";
}
int32_t NotFoundServlet::handle(HttpRequest::ptr request,
                                HttpResponse::ptr response,
                                HttpSession::ptr session) {
    response->setHttpStatus(HttpStatus::NOT_FOUND);
    response->setConnection(false);
    response->setHeader("Content-Type", "text/html");
    // m_content.replace(m_content.find("{URL}"), 5, request->getPath());
    response->setBody(m_content);
    return 0;
}

} // namespace http

} // namespace lyon
