#include "lyon/iomanager.h"
#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <fcntl.h>
#include <lyon/log.h>
#include <string.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_socket() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "192.168.2.3", &addr.sin_addr.s_addr);
    if (!connect(sock, (sockaddr *)&addr, sizeof(addr))) {

    } else if (errno == EINPROGRESS) {
        LYON_LOG_INFO(g_logger) << "add read event";
        lyon::IOManager::GetCurrentIOManager()->addEvent(
            sock, lyon::IOManager::READ,
            []() { LYON_LOG_INFO(g_logger) << "read"; });

        LYON_LOG_INFO(g_logger) << "add write event";
        lyon::IOManager::GetCurrentIOManager()->addEvent(
            sock, lyon::IOManager::WRITE,
            []() { LYON_LOG_INFO(g_logger) << "write"; });
    } else {
        LYON_LOG_INFO(g_logger) << "ukonwn";
    }
}

void test1() {
    std::cout << "EPOLLIN=" << EPOLLIN << " EPOLLOUT=" << EPOLLOUT << std::endl;
    lyon::IOManager iom(1, false);
    iom.addJob(test_socket);
}

lyon::Timer::ptr timer;

void test_timer() {
    lyon::IOManager iom(2);
    timer = iom.addTimer(
        1000,
        []() {
            static int i = 0;
            LYON_LOG_INFO(g_logger) << "hello timer i=" << i;
            if (++i == 3) {
                timer->reset(2000, true);
                // s_timer->cancel();
            }
        },
        true);
}

int main(int argc, char *argv[]) {
    // test1();
    test_timer();
    return 0;
}
