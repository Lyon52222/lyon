#include "scheduler.h"
#include "log.h"
#include "macro.h"
#include "util.h"
#include <cstddef>
#include <string>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static thread_local Scheduler *t_current_scheduler = nullptr;
static thread_local Fiber *t_scheduler_fiber = nullptr;

Scheduler::Scheduler(size_t threads, bool use_caller, const std::string &name)
    : m_name(name) {
    LYON_ASSERT(threads > 0);

    //是否将自身在协程中运行
    if (use_caller) {
        threads--;
        LYON_ASSERT(GetCurrentScheduler() == nullptr);
        t_current_scheduler = this;
        m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this), 0, true));
        Thread::SetName(m_name);

        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread = GetCurrentThreadId();

        //将创建协程的线程也放入线程池中
        m_threadIds.push_back(m_rootThread);
    } else {
        m_rootThread = -1;
    }
    m_threadCount = threads;
}

Scheduler::~Scheduler() {
    LYON_ASSERT(m_stopping);
    if (t_current_scheduler == this) {
        t_current_scheduler = nullptr;
    }
}

void Scheduler::start() {
    MutexType::Lock lock(m_mutex);
    if (!m_stopping) {
        return;
    }
    m_stopping = false;
    LYON_ASSERT(m_threads.empty());
    m_threads.resize(m_threadCount);
    for (size_t i = 0; i < m_threadCount; i++) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this),
                                      m_name + "_" + std::to_string(i)));

        m_threadIds.push_back(m_threads[i]->getId());
    }
    lock.unlock();
}

void Scheduler::stop() {
    m_autoStop = true;
    //创建scheduler的协程
    if (m_rootFiber && m_threadCount == 0 &&
        (m_rootFiber->getState() == Fiber::TERM ||
         m_rootFiber->getState() == Fiber::INIT)) {
        LYON_LOG_INFO(g_logger) << this->m_name << " stopped";
        m_stopping = true;
        if (stopping()) {
            return;
        }
    }

    if (m_rootThread == -1) {
        LYON_ASSERT(GetCurrentScheduler() != this);
    } else {
        LYON_ASSERT(GetCurrentScheduler() == this);
    }
}

void Scheduler::run() {
    LYON_LOG_DEBUG(g_logger) << m_name << " start run";
    setAsCurrentScheduler();
}

void Scheduler::tickle() {}

Fiber *Scheduler::GetMainFiber() { return t_scheduler_fiber; }
Scheduler *Scheduler::GetCurrentScheduler() { return t_current_scheduler; }
void Scheduler::setAsCurrentScheduler() { t_current_scheduler = this; }
} // namespace lyon
