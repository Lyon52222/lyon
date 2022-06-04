#include "lyon/http/http_connection.h"
#include "lyon/http/http_protocol.h"
#include "lyon/uri.h"
#include <lyon/iomanager.h>
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test() {
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://localhost:8088/echo");
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://localhost:8088/");
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://www.baidu.com:80/");
    lyon::Uri::ptr uri = lyon::Uri::Parser("http://www.sylar.top:80/blog/");
    std::map<std::string, std::string> headers;

    lyon::http::HttpResult::ptr rt = lyon::http::HttpConnection::DoRequest(
        lyon::http::HttpMethod::GET, uri, headers, "hi", 4000);

    LYON_LOG_INFO(g_logger) << rt->toString();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    iom.addJob(test);
    return 0;
}
