#include "scheduler.h"
#include "fiber.h"
#include "hook.h"
#include "log.h"
#include "macro.h"
#include "util.h"
#include <cstddef>
#include <string>
namespace lyon {

static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static thread_local Scheduler *t_current_scheduler = nullptr;
static thread_local Fiber *t_scheduler_fiber = nullptr;

/**
 * @brief 实例化调度器
 *
 * @param threads 线程池数
 * @param use_caller 是否将实例化调度器的线程设置为main fiber
 * @param name 调度器名称
 */
Scheduler::Scheduler(size_t threads, bool join_fiber, const std::string &name)
    : m_name(name) {
    LYON_ASSERT(threads > 0);

    //是否将当前线程也加入到线程池中
    if (join_fiber) {
        // Fiber::GetMainFiber();
        threads--;
        LYON_ASSERT(GetCurrentScheduler() == nullptr);
        t_current_scheduler = this;
        m_runFiber.reset(new Fiber(std::bind(&Scheduler::run, this), false));

        t_scheduler_fiber = m_runFiber.get();
        m_rootThread = GetCurrentThreadId();

        lyon::Thread::SetName(m_name + "0");
        // m_threadIds.push_back(m_rootThread);
        m_threadIds.push_back(GetCurrentThreadId());

    } else {
        m_rootThread = static_cast<pthread_t>(-1);
        m_runFiber = nullptr;
    }
    m_threadCount = threads;
}

Scheduler::~Scheduler() {
    LYON_ASSERT(m_stopping);
    if (GetCurrentScheduler() == this) {
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
        //这里创建的所有线程都是主协程运行的run，子协程为运行的任务
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this),
                                      m_name + "_" + std::to_string(i + 1)));

        m_threadIds.push_back(m_threads[i]->getId());
    }
    lock.unlock();
}

void Scheduler::stop() {
    m_autoStop = true;
    // 实例化调度器时的参数 use_caller 为 true, 并且指定线程数量为 1 时
    // 说明只有当前一条主线程在执行，简单等待执行结束即可

    if (m_runFiber && m_threadCount == 0 &&
        (m_runFiber->getState() == Fiber::TERM ||
         m_runFiber->getState() == Fiber::INIT)) {
        LYON_LOG_INFO(g_logger) << this->m_name << " stopped";
        m_stopping = true;
        if (stopping()) {
            return;
        }
    }

    if (m_rootThread == static_cast<pthread_t>(-1)) {
        LYON_ASSERT(GetCurrentScheduler() != this);
    } else {
        //说明这是usecaller创建的线程
        LYON_ASSERT(GetCurrentScheduler() == this);
    }

    m_stopping = true;
    // for (size_t i = 0; i < m_threadCount; i++) {
    //     tickle();
    // }
    if (m_runFiber) {
        tickle();
    }

    //如果当前线程是调度线程的运行协程，则切换到run协程。
    if (m_runFiber) {
        if (!stopping()) {
            m_runFiber->mainFiberIn();
        }
    }

    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }

    for (auto &thr : thrs) {
        thr->join();
    }
}

void Scheduler::run() {
    LYON_LOG_DEBUG(g_logger) << m_name << " start run";
    lyon::set_hook_enable(true);
    setAsCurrentScheduler();

    if (lyon::GetCurrentThreadId() != m_rootThread) {
        t_scheduler_fiber = Fiber::GetCurrentFiber().get();
    }

    Job job;
    Fiber::ptr cb_fiber;
    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
    while (true) {
        //线程是否还处于活跃状态(是否还有任务需要执行)
        bool is_active = false;
        //是否触发tickle用于通知其他线程
        bool is_tickle = false;
        job.reset();
        {
            //在任务列表中寻找可调度任务
            MutexType::Lock lock(m_mutex);
            auto j_itr = m_jobs.begin();
            while (j_itr != m_jobs.end()) {
                //-1表示为指定在哪个线程执行
                //,当前线程并不是协程指定运行的线程
                if (j_itr->specified_thread &&
                    j_itr->thread != lyon::GetCurrentThreadId()) {
                    j_itr++;
                    //通知其他线程来处理
                    is_tickle = true;
                    continue;
                }

                //只能对协程或函数进行调度
                LYON_ASSERT(j_itr->cb || j_itr->fiber);

                //如果协程正在执行中，则不需要被调度
                if (j_itr->fiber && j_itr->fiber->getState() == Fiber::EXEC) {
                    j_itr++;
                    continue;
                }

                job = *j_itr;
                m_jobs.erase(j_itr);
                m_activeThreadCount++;
                is_active = true;
                break;
            }
        }

        if (is_tickle) {
            tickle();
        }

        //需要被调度的任务是协程且仍为终止
        if (job.fiber && (job.fiber->getState() != Fiber::TERM ||
                          job.fiber->getState() != Fiber::EXCEPT)) {
            // job.fiber->SetMainFiber(Fiber::GetMainFiber());
            // job.fiber->mainFiberIn();
            job.fiber->schedulerIn();
            m_activeThreadCount--;

            if (job.fiber->getState() == Fiber::READY) {
                addJob(job.fiber);
            } else if (job.fiber->getState() != Fiber::TERM &&
                       job.fiber->getState() != Fiber::EXCEPT) {
                job.fiber->m_state = Fiber::HOLD;
            }
            job.reset();

        }
        //需要被调度的任务是函数
        else if (job.cb) {
            if (cb_fiber) {
                cb_fiber->reset(job.cb);
            } else {
                cb_fiber.reset(new Fiber(job.cb));
            }
            job.reset();
            // cb_fiber->mainFiberIn();
            cb_fiber->schedulerIn();
            m_activeThreadCount--;
            if (cb_fiber->getState() == Fiber::READY) {
                addJob(cb_fiber);
                cb_fiber.reset();
            } else if (cb_fiber->getState() == Fiber::TERM ||
                       cb_fiber->getState() == Fiber::EXCEPT) {
                cb_fiber->reset(nullptr);
            } else {
                cb_fiber->m_state = Fiber::HOLD;
                cb_fiber.reset();
            }
        }
        //无可调度任务
        else {
            if (is_active) {
                m_activeThreadCount--;
                continue;
            }

            if (idle_fiber->getState() == Fiber::TERM ||
                idle_fiber->getState() == Fiber::EXCEPT) {
                LYON_LOG_INFO(g_logger) << "idle fiber term";
                break;
            }

            m_idleThreadCount++;
            // idle_fiber->mainFiberIn();
            idle_fiber->schedulerIn();
            m_idleThreadCount--;
        }
    }
}

void Scheduler::tickle() {
    LYON_LOG_INFO(g_logger) << "tickle Fiber id = " << GetCurrentFiberId();
}

void Scheduler::idle() {
    LYON_LOG_INFO(g_logger) << "idle Fiber id = " << GetCurrentFiberId();
    while (!stopping()) {
        lyon::Fiber::HoldToScheduler();
    }
}

bool Scheduler::stopping() {
    MutexType::Lock lock(m_mutex);
    return m_stopping && m_autoStop && m_jobs.empty() &&
           m_activeThreadCount == 0;
}

Scheduler *Scheduler::GetCurrentScheduler() { return t_current_scheduler; }
Fiber *Scheduler::GetMainFiber() { return t_scheduler_fiber; }
void Scheduler::setAsCurrentScheduler() { t_current_scheduler = this; }
} // namespace lyon
