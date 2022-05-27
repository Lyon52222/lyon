#include "lyon/http/http.h"
#include "lyon/log.h"
#include <lyon/address.h>
#include <lyon/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_http_request() {
    lyon::http::HttpRequest::ptr request(new lyon::http::HttpRequest());
    request->setHeader("Host", "www.baidu.com");
    std::cout << *request;

    lyon::IPAddress::ptr bd =
        lyon::Address::LookUpAnyIpAddress("www.baidu.com:80");
    lyon::Socket::ptr sock = lyon::Socket::CreateTCPSocket();
    sock->connect(bd);
    std::string str = request->toString();

    sock->send(str.c_str(), str.size());

    str.resize(4096);
    int rt = sock->recv(&str[0], str.size());
    str.resize(rt);

    LYON_LOG_INFO(g_logger) << str;
}

int main(int argc, char *argv[]) {
    test_http_request();

    lyon::http::HttpResponse::ptr response(new lyon::http::HttpResponse());
    std::cout << *response;

    return 0;
}
