#include "lyon/log.h"
#include "lyon/scheduler.h"
#include <lyon/util.h>
static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void func() {
    static int s_count = 5;
    LYON_LOG_INFO(g_logger) << "test in fiber s_count = " << s_count;
    sleep(1);
    if (s_count-- > 0) {
        lyon::Scheduler::GetCurrentScheduler()->scheduleWithLock(
            &func, lyon::GetCurrentThreadId());
    }
}

int main(int argc, char *argv[]) {

    LYON_LOG_INFO(g_logger) << "scheduler init";
    lyon::Scheduler scheduler(1, false, "main");

    LYON_LOG_INFO(g_logger) << "scheduler start";
    scheduler.start();

    LYON_LOG_INFO(g_logger) << "add job";
    scheduler.scheduleWithLock(func);

    scheduler.stop();

    LYON_LOG_INFO(g_logger) << "end";
    return 0;
}
