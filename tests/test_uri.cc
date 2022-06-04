#include "lyon/uri.h"
#include <iostream>

void test_uri() {
    std::string str =
        "http://user:pass@www.example.com:8088/home/index.html?name=lyon#abc";
    lyon::Uri::ptr uri = lyon::Uri::Parser(str);

    if (uri) {
        std::cout << str << std::endl;

        std::cout << "Scheme = " << uri->getScheme() << std::endl;
        std::cout << "Userinfo = " << uri->getUserinfo() << std::endl;
        std::cout << "Host = " << uri->getHost() << std::endl;
        std::cout << "Port = " << uri->getPort() << std::endl;
        std::cout << "Path = " << uri->getPath() << std::endl;
        std::cout << "Query = " << uri->getQuery() << std::endl;
        std::cout << "Fragment = " << uri->getFragment() << std::endl;

        std::cout << uri->toString() << std::endl;
    }
}

void test_path() {
    std::string str = "/home/index.html?name=lyon#abc";
    lyon::Uri::ptr uri = lyon::Uri::Parser(str);
    if (uri) {
        std::cout << str << std::endl;

        std::cout << "Scheme = " << uri->getScheme() << std::endl;
        std::cout << "Userinfo = " << uri->getUserinfo() << std::endl;
        std::cout << "Host = " << uri->getHost() << std::endl;
        std::cout << "Port = " << uri->getPort() << std::endl;
        std::cout << "Path = " << uri->getPath() << std::endl;
        std::cout << "Query = " << uri->getQuery() << std::endl;
        std::cout << "Fragment = " << uri->getFragment() << std::endl;
        std::cout << uri->toString() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    test_uri();
    test_path();
    return 0;
}
