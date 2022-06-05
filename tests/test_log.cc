#include "lyon/config.h"
#include "lyon/log.h"
#include <memory>
#include <thread>

static lyon::Logger::ptr root_logger = LYON_LOG_GET_ROOT();
static lyon::Logger::ptr sys_logger = LYON_LOG_GET_LOGGER("system");

void root_log() {
    for (int i = 0; i < 1000; i++) {
        LYON_LOG_INFO(root_logger) << "12345678910";
        // sleep(1);
        // std::cout << "12345678910" << std::endl;
        LYON_LOG_INFO(root_logger) << "10987654321";
    }
}

void system_log() {
    for (int i = 0; i < 1000; i++) {
        LYON_LOG_INFO(sys_logger) << "abcdefghijk";
        // sleep(1);
        LYON_LOG_INFO(sys_logger) << "ABCDEFGHIJK";
        // std::cout << "abcdefghijk" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    lyon::Config::LoadFromConfigFile("/home/lyon/code/C++/lyon/conf/log.yml");
    LYON_LOG_DEBUG(root_logger) << "this is a test";

    std::thread t1(root_log);
    std::thread t2(system_log);
    t1.join();
    t2.join();

    return 0;
}
