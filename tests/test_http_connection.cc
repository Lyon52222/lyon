#include "lyon/http/http_connection.h"
#include "lyon/http/http_protocol.h"
#include "lyon/uri.h"
#include <lyon/iomanager.h>
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_connectionpool() {
    lyon::http::HttpConnectionPool::ptr pool =
        lyon::http::HttpConnectionPool::Create("www.baidu.com", "www.baidu.com",
                                               80, 10, 1000, 3);

    lyon::IOManager::GetCurrentIOManager()->addTimer(
        1000,
        [pool]() {
            lyon::http::HttpResult::ptr rt = pool->doGet("/", 1000);

            LYON_LOG_INFO(g_logger) << rt->toString();
        },
        true);
}

void test() {
    lyon::Uri::ptr uri = lyon::Uri::Parser("http://localhost:8088/echo");
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://localhost:8088/");
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://www.baidu.com:80/");
    // chunk
    // lyon::Uri::ptr uri = lyon::Uri::Parser("http://www.sylar.top:80/blog/");

    std::map<std::string, std::string> headers;

    lyon::IOManager iom(2);
    iom.addTimer(
        1000,
        [uri, headers]() {
            lyon::http::HttpResult::ptr rt =
                lyon::http::HttpConnection::DoRequest(
                    lyon::http::HttpMethod::GET, uri, 4000, headers, "hi");

            LYON_LOG_INFO(g_logger) << rt->toString();
        },
        true);
}

int main(int argc, char *argv[]) {
    // lyon::IOManager iom(1);
    // iom.addJob(test);
    test();
    // iom.addJob(test_connectionpool);
    return 0;
}
