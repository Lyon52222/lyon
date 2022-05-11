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
        // lyon::Scheduler::GetCurrentScheduler()->addJob(
        //     func1, lyon::GetCurrentThreadId());
    }
}

int main(int argc, char *argv[]) {

    LYON_LOG_INFO(g_logger) << "scheduler init";
    lyon::Scheduler scheduler(1, false, "test");

    LYON_LOG_INFO(g_logger) << "scheduler start";
    scheduler.start();

    LYON_LOG_INFO(g_logger) << "add func job";
    scheduler.addJob(func);

    LYON_LOG_INFO(g_logger) << "add fiber job";
    // BUG:这里还会有问题。如果这了创建fiber为任务的话。回为当前线程创建一个主要fiber并且永远也不会返回。
    // 这样在析构的时候主fiber的状态还会是Exce
    lyon::Fiber::ptr fiber(new lyon::Fiber(func1));
    scheduler.addJob(fiber);

    scheduler.stop();
    LYON_LOG_INFO(g_logger) << "end";
    return 0;
}
