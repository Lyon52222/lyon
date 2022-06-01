#include "lyon/http/http_server.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/tcp_server.h>

void test_http_server() {
    lyon::Address::ptr local = lyon::Address::LookUpAny("0.0.0.0:8088");
    lyon::TcpServer::ptr server(new lyon::http::HttpServer());

    server->bindAndListen(local);

    server->start();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(3);
    iom.addJob(test_http_server);
    return 0;
}
