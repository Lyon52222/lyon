#include "thread.h"
#include "log.h"

namespace lyon {

static thread_local Thread *t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOWN";

static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

std::string &Thread::GetName() { return t_thread_name; }

void Thread::SetName(const std::string &name) {
    if (name.empty())
        return;
    if (t_thread) {
        t_thread->m_name = name;
    }
    t_thread_name = name;
}

void Thread::join() {
    if (m_thread) {
        int rt = pthread_join(m_thread, nullptr);
        if (rt) {
            LYON_LOG_ERROR(g_logger)
                << "Pthread join thread: " << m_name << " fail, rt = " << rt;
            throw std::logic_error("Pthread join error");
        }
        m_thread = 0;
    }
}

void *Thread::run(void *arg) {
    Thread *thread = (Thread *)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    //此时已经处于新线程中了，这时获取的是新线程的id
    thread->m_id = GetCurrentThreadId();

    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    //这一步是通知父线程，子线程已经初始化完成，准备就是，让父线程返回
    thread->m_semaphore.notify();
    cb();
    return 0;
}

Thread *Thread::GetThis() { return t_thread; }

Thread::Thread(std::function<void()> cb, const std::string &name)
    : m_name(name), m_cb(cb) {
    if (name.empty()) {
        m_name = "UNKNOWN";
    }

    //此处创建一个新的线程，并在新线程中运行run函数
    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "Pthread create thread: " << name << " fail, rt = " << rt;
        throw std::logic_error("Pthread create error");
    }
    //这一步是为了等待子线程创建完成。
    m_semaphore.wait();
}

Thread::~Thread() {
    if (m_thread) {
        pthread_detach(m_thread);
    }
}
} // namespace lyon
