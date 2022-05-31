#include "lyon/iomanager.h"
#include "lyon/log.h"
#include "lyon/tcp_server.h"
#include <lyon/address.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void run() {
    lyon::Address::ptr bd =
        lyon::Address::LookUpAny("0.0.0.0:8088", AF_INET, SOCK_STREAM);

    // lyon::UnixAddress::ptr addr(new lyon::UnixAddress("/tmp/unix_addr"));

    std::vector<lyon::Address::ptr> addrs;
    std::vector<lyon::Address::ptr> fails;
    addrs.push_back(bd);
    // addrs.push_back(addr);
    lyon::TcpServer::ptr server(new lyon::TcpServer());

    while (!server->bindAndListen(addrs, fails)) {
        sleep(2);
    }
    server->start();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    iom.addJob(run);

    return 0;
}
