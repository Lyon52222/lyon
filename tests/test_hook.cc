#include "arpa/inet.h"
#include "iostream"
#include "lyon/hook.h"
#include "lyon/iomanager.h"
#include "sys/socket.h"
#include "unistd.h"
#include <lyon/log.h>
#include <string.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_sleep() {
    lyon::IOManager iom(1, true, "hook_test");
    iom.addJob([]() {
        sleep(2);
        LYON_LOG_INFO(g_logger) << "sleep 2";
    });

    iom.addJob([]() {
        sleep(3);
        LYON_LOG_INFO(g_logger) << "sleep 3";
    });
    LYON_LOG_INFO(g_logger) << "test_sleep";
}

void test_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "14.215.177.38", &addr.sin_addr.s_addr);

    LYON_LOG_INFO(g_logger) << "try connect";
    int rt = connect(sock, (const struct sockaddr *)&addr, sizeof(addr));
    if (!rt) {
        LYON_LOG_INFO(g_logger) << "connect success";
    } else {
        return;
    }

    const char head[] = "GET / HTTP/1.0\r\n\r\n";

    LYON_LOG_INFO(g_logger) << "try send";
    rt = send(sock, head, sizeof(head), 0);
    if (rt == -1) {
        LYON_LOG_INFO(g_logger) << "send fail errno = " << errno;
        return;
    } else {
        LYON_LOG_INFO(g_logger) << "send success size = " << rt;
    }

    std::string buff;
    buff.resize(4096);

    rt = recv(sock, &buff[0], buff.size(), 0);
    if (rt == -1) {
        return;
    }

    buff.resize(rt);

    LYON_LOG_INFO(g_logger) << std::endl << buff;

    if (rt <= 0) {
        return;
    }
}

int main(int argc, char *argv[]) {
    // test_sleep();
    lyon::IOManager iom;
    iom.addJob(test_socket);
    return 0;
}
