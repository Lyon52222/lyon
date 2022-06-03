#include "lyon/socket.h"
#include "lyon/ssl_socket.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/log.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

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

void test_sslsocket() {
    lyon::IPAddress::ptr bd = lyon::Address::LookUpAnyIpAddress(
        "www.baidu.com:80", AF_INET, SOCK_STREAM);

    lyon::Socket::ptr sock = lyon::SSLSocket::CreateTCPSocket();

    sock->connect(bd);

    const char buffer[] = "GET / HTTP/1.1\r\n\r\n";

    sock->send(buffer, sizeof(buffer));

    std::string buffers;
    buffers.resize(4096);

    int rt = sock->recv(&buffers[0], buffers.size());
    if (rt > 0) {
        buffers.resize(rt);
        LYON_LOG_INFO(g_logger) << buffers;
    } else {
        LYON_LOG_INFO(g_logger) << "recv error";
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom;
    // iom.addJob(test_socket);
    test_sslsocket();
    // test_socket();
    return 0;
}
