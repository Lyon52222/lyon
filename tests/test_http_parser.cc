#include "lyon/http/http_parser.h"
#include "lyon/socket.h"
#include <iostream>
#include <lyon/address.h>
#include <lyon/http/http.h>

void test_request_parser() {
    const char test_request_data[] = "POST / HTTP/1.1\r\n"
                                     "Host: www.test.com\r\n"
                                     "Content-Length: 10\r\n\r\n"
                                     "1234567890";

    lyon::http::HttpRequestParser::ptr httprequestparser(
        new lyon::http::HttpRequestParser());
    httprequestparser->excute(test_request_data, sizeof(test_request_data), 0);
    if (!httprequestparser->hasError())
        std::cout << httprequestparser->getData()->toString() << std::endl;
}

void test_response_parser() {
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

    httpresponseparser->excute(&str[0], str.size(), 0);

    std::cout << httpresponseparser->getData()->toString() << std::endl;
}

int main(int argc, char *argv[]) {
    test_request_parser();
    // test_response_parser();
    return 0;
}
