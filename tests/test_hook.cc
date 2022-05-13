#include "iostream"
#include "lyon/hook.h"
#include "lyon/iomanager.h"
#include "unistd.h"
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_sleep() {
    lyon::IOManager iom(1);
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

int main(int argc, char *argv[]) {
    test_sleep();
    return 0;
}
