#include "lyon/http/http_parser.h"
#include "lyon/socket.h"
#include <iostream>
#include <lyon/address.h>
#include <lyon/http/http.h>
int main(int argc, char *argv[]) {
    lyon::Socket::ptr sock = lyon::Socket::CreateTCPSocket();
    lyon::IPAddress::ptr bd =
        lyon::Address::LookUpAnyIpAddress("www.baidu.com:80");
    sock->connect(bd);

    lyon::http::HttpRequest::ptr request(new lyon::http::HttpRequest());

    std::string str = request->toString();

    sock->send(str.c_str(), str.size());

    str.resize(4096);
    int rt = sock->recv(&str[0], str.size());
    str.resize(rt);
    std::cout << str << std::endl;

    lyon::http::HttpResponseParser::ptr httpresponseparser(
        new lyon::http::HttpResponseParser());

    httpresponseparser->excute(&str[0], str.size());

    std::cout << httpresponseparser->getData()->toString() << std::endl;

    return 0;
}
