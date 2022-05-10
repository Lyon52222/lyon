#include "lyon/log.h"
#include "lyon/scheduler.h"
#include <lyon/util.h>
static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void func() {
    static int s_count = 5;
    LYON_LOG_INFO(g_logger) << "func0 in fiber s_count = " << s_count;
    sleep(1);
    if (s_count-- > 0) {
        //指定后面继续在当前线程中执行
        lyon::Scheduler::GetCurrentScheduler()->addJob(
            func, true, lyon::GetCurrentThreadId());
    }
}

void func1() {
    static int s_count = 5;
    LYON_LOG_INFO(g_logger) << "func1 in fiber s_count = " << s_count;
    sleep(1);
    if (s_count-- > 0) {
        lyon::Scheduler::GetCurrentScheduler()->addJob(
            &func1, lyon::GetCurrentThreadId());
    }
}
int main(int argc, char *argv[]) {

    LYON_LOG_INFO(g_logger) << "scheduler init";
    lyon::Scheduler scheduler(3, true, "test");

    LYON_LOG_INFO(g_logger) << "scheduler start";
    scheduler.start();

    LYON_LOG_INFO(g_logger) << "add job";
    scheduler.addJob(func);
    // scheduler.addJob(func1);

    scheduler.stop();
    LYON_LOG_INFO(g_logger) << "end";
    return 0;
}
