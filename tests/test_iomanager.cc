#include "lyon/iomanager.h"
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <fcntl.h>
#include <lyon/log.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_socket() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "192.168.2.12", &addr.sin_addr.s_addr);
    if (!connect(sock, (sockaddr *)&addr, sizeof(addr))) {

    } else if (errno == EINPROGRESS) {
        lyon::IOManager::GetCurrentIOManager()->addEvent(
            sock, lyon::IOManager::READ,
            []() { LYON_LOG_INFO(g_logger) << "read"; });
        lyon::IOManager::GetCurrentIOManager()->addEvent(
            sock, lyon::IOManager::WRITE,
            []() { LYON_LOG_INFO(g_logger) << "write"; });
    } else {
        LYON_LOG_INFO(g_logger) << "ukonwn";
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(3, false, "iomanager");
    test_socket();
    return 0;
}
