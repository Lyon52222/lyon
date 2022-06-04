#include "lyon/http/http_protocol.h"
#include "lyon/http/http_server.h"
#include <cstdint>
#include <lyon/address.h>
#include <lyon/http/http_servlet.h>
#include <lyon/http/http_session.h>
#include <lyon/iomanager.h>
#include <lyon/tcp_server.h>

void test_http_server() {
    lyon::Address::ptr local = lyon::Address::LookUpAny("0.0.0.0:8088");
    lyon::http::HttpServer::ptr server(new lyon::http::HttpServer());
    lyon::http::ServletDispatch::ptr servlet(new lyon::http::ServletDispatch());

    lyon::http::FunctionalServlet::callback echo_servlet =
        [](lyon::http::HttpRequest::ptr request,
           lyon::http::HttpResponse::ptr response,
           lyon::http::HttpSession::ptr session) -> int32_t {
        response->setBody(request->getBody());
        return 0;
    };

    // lyon::http::FunctionalServlet::callback chunk_servlet =
    //     [](lyon::http::HttpRequest::ptr request,
    //        lyon::http::HttpResponse::ptr response,
    //        lyon::http::HttpSession::ptr session) -> int32_t {
    //     response->setHeader("Transfer-Encoding", "chunked");
    //     response->setBody("3\r\n123\r\n5\r\nabcde");
    //     return 0;
    // };

    servlet->addAccurateServlet("/echo", echo_servlet);
    // servlet->addAccurateServlet("/chunk", chunk_servlet);

    server->setServletDispatch(servlet);

    server->bindAndListen(local);

    server->start();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(3);
    iom.addJob(test_http_server);
    return 0;
}
