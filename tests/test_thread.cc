#include "lyon/thread.h"
#include <iostream>
#include <lyon/log.h>
#include <lyon/mutex.h>
#include <lyon/util.h>

lyon::Mutex m_mutex;
lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int count = 0;

void fun1() {
    LYON_LOG_INFO(g_logger)
        << "name: " << lyon::Thread::GetName()
        << "\t this.name: " << lyon::Thread::GetThis()->getName()
        << "\t id: " << lyon::GetCurrentThreadId()
        << "\t this.id: " << lyon::Thread::GetThis()->getId();

    for (int i = 0; i < 100000; i++) {
        lyon::Mutex::Lock lock(m_mutex);
        count++;
    }
}

int main(int argc, char *argv[]) {
    lyon::Thread thread_a(fun1, "function1");
    lyon::Thread thread_b(fun1, "function2");
    lyon::Thread thread_c(fun1, "function3");
    lyon::Thread thread_d(fun1, "function4");
    thread_a.join();
    thread_b.join();
    thread_c.join();
    thread_d.join();
    LYON_LOG_INFO(g_logger) << "thread test end";
    LYON_LOG_INFO(g_logger) << "count = " << count;
    return 0;
}
