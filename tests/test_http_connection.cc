#include "lyon/http/http_connection.h"
#include "lyon/http/http_protocol.h"
#include "lyon/uri.h"

void test() {
    lyon::Uri::ptr uri = lyon::Uri::Parser("http://www.baidu.com:80/");
    std::map<std::string, std::string> headers = {{"Content-Length", "2"}};

    lyon::http::HttpConnection::DoRequest(lyon::http::HttpMethod::GET, uri,
                                          headers, "hi", 3000);
}

int main(int argc, char *argv[]) {
    test();
    return 0;
}
