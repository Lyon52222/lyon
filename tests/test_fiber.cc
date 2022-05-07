#include "lyon/fiber.h"
#include "lyon/thread.h"
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void fiber_fun1() {
    LYON_LOG_INFO(g_logger) << "Fiber In : " << lyon::Fiber::GetFiberId();
    lyon::Fiber::GetCurrentFiber()->back();
    LYON_LOG_INFO(g_logger) << "Fiber Out : " << lyon::Fiber::GetFiberId();
}

int main(int argc, char *argv[]) {
    lyon::Fiber::GetCurrentFiber();
    lyon::Fiber::ptr fiber(new lyon::Fiber(fiber_fun1));

    LYON_LOG_INFO(g_logger) << "Fiber count = " << lyon::Fiber::TotalFibers();
    fiber->call();
    LYON_LOG_INFO(g_logger)
        << "Main after swapin : " << lyon::Fiber::GetFiberId();
    fiber->call();
    LYON_LOG_INFO(g_logger)
        << "Main after swapout : " << lyon::Fiber::GetFiberId();
    return 0;
}
