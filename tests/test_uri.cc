#include "lyon/uri.h"
#include <iostream>
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_uri() {
    std::string str =
        "http://user:pass@www.example.com:8088/home/index.html?name=lyon#abc";
    lyon::Uri::ptr uri = lyon::Uri::Parser(str);

    if (uri) {
        LYON_LOG_INFO(g_logger) << str;

        LYON_LOG_INFO(g_logger) << "Scheme = " << uri->getScheme();
        LYON_LOG_INFO(g_logger) << "Userinfo = " << uri->getUserinfo();
        LYON_LOG_INFO(g_logger) << "Host = " << uri->getHost();
        LYON_LOG_INFO(g_logger) << "Port = " << uri->getPort();
        LYON_LOG_INFO(g_logger) << "Path = " << uri->getPath();
        LYON_LOG_INFO(g_logger) << "Query = " << uri->getQuery();
        LYON_LOG_INFO(g_logger) << "Fragment = " << uri->getFragment();

        LYON_LOG_INFO(g_logger) << uri->toString();
    }
}

void test_path() {
    std::string str = "/home/index.html?name=lyon#abc";
    lyon::Uri::ptr uri = lyon::Uri::Parser(str);
    if (uri) {
        LYON_LOG_INFO(g_logger) << str;

        LYON_LOG_INFO(g_logger) << "Scheme = " << uri->getScheme();
        LYON_LOG_INFO(g_logger) << "Userinfo = " << uri->getUserinfo();
        LYON_LOG_INFO(g_logger) << "Host = " << uri->getHost();
        LYON_LOG_INFO(g_logger) << "Port = " << uri->getPort();
        LYON_LOG_INFO(g_logger) << "Path = " << uri->getPath();
        LYON_LOG_INFO(g_logger) << "Query = " << uri->getQuery();
        LYON_LOG_INFO(g_logger) << "Fragment = " << uri->getFragment();
        LYON_LOG_INFO(g_logger) << uri->toString();
    }
}

int main(int argc, char *argv[]) {
    test_uri();
    test_path();
    return 0;
}
