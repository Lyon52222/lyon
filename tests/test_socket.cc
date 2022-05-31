#include "lyon/socket.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

// BUG: 在iomanager的任务函数中使用，或递归使用了 getsockopt 等时会发生段错误
void test_socket() {
    // LYON_LOG_INFO(g_logger) << "test";

    lyon::IPAddress::ptr bd =
        lyon::Address::LookUpAnyIpAddress("www.baidu.com:80");

    // lyon::IPv4Address::ptr bd = lyon::IPv4Address::Create("14.215.177.38",
    // 80);

    lyon::Socket::ptr sock = lyon::Socket::CreateTCPSocket();

    sock->connect(bd);

    const char buffer[] = "GET / HTTP/1.0\r\n\r\n";

    sock->send(buffer, sizeof(buffer));

    std::string buffers;
    buffers.resize(4096);

    int rt = sock->recv(&buffers[0], buffers.size());
    buffers.resize(rt);

    LYON_LOG_INFO(g_logger) << buffers;
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom;
    iom.addJob(test_socket);
    // test_socket();
    return 0;
}
