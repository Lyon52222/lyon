#include "lyon/http/http.h"
#include "lyon/log.h"

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int main(int argc, char *argv[]) {
    lyon::http::HttpRequest::ptr request(new lyon::http::HttpRequest());
    request->setHeader("Host", "www.baidu.com");
    std::cout << *request;

    lyon::http::HttpResponse::ptr response(new lyon::http::HttpResponse());
    std::cout << *response;

    return 0;
}
