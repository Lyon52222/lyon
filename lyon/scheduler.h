#ifndef __LYON_SCHEDULER_H__
#define __LYON_SCHEDULER_H__

#include "fiber.h"
#include "log.h"
#include "mutex.h"
#include "thread.h"
#include <functional>
#include <list>
#include <memory>
#include <vector>
namespace lyon {

/**
 * @brief 协程调度器，负责对协程进行调度
 * 整个调度的思路是。调度器创建n个线程。每个线程都运行run函数。
 * run函数遍历需要被调度的工作，并将其运行。
 */
class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    /**
     * @brief 调度器构造函数
     *
     * @param threads 线程数量
     * @param join_fiber
     * 是否将创建调度器的线程加入线程池中,并且将当前代码视作一个工作协程加入调度
     * @param name 调度器名称
     */
    Scheduler(size_t threads = 1, bool join_fiber = true,
              const std::string &name = "");

    virtual ~Scheduler();

    /**
     * @brief 创建并运行线程池
     *
     */
    void start();
    void stop();
    /**
     * @brief 线程池中的线程运行的函数。用于对job进行调度
     *
     */
    void run();

    /**
     * @brief 加锁添加调度任务
     *
     * @tparam FiberOrCb 任务类型
     * @param f 调度任务
     * @param s_thread 是否在指定线程上执行
     * @param thread 任务在哪个线程上执行
     */
    template <class FiberOrFunc>
    void addJob(FiberOrFunc f, bool s_thread = false, pthread_t thread = 0) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleWithoutLock(f, s_thread, thread);
        }
        if (need_tickle)
            tickle();
    }

    /**
     * @brief 加锁批量添加调度任务
     *
     * @tparam InputIterator 迭代器类型
     * @param begin 开始迭代器
     * @param end 结束迭代器
     * @param s_thread 是否在指定线程上执行
     * @param thread 任务在哪个线程上执行
     */
    template <class InputIterator>
    void addJobs(InputIterator begin, InputIterator end, bool s_thread = false,
                 pthread_t thread = 0) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while (begin != end) {
                need_tickle = scheduleWithoutLock(*begin, s_thread, thread) ||
                              need_tickle;
                begin++;
            }
        }
        if (need_tickle)
            tickle();
    }

public:
    // static Fiber *GetMainFiber();
    /**
     * @brief 获取当前线程的调度器
     *
     */
    static Scheduler *GetCurrentScheduler();

protected:
    virtual void tickle();
    /**
     * @brief 无任务可调度时执行
     *
     */
    virtual void idle();
    /**
     * @brief 虚函数，子类实现用于做一些清理收尾工作
     *
     */
    virtual bool stopping();

private:
    void setAsCurrentScheduler();

    /**
     * @brief 添加调度任务
     *
     * @tparam FiberOrCb 任务类型
     * @param f 任务
     * @param thread 任务需要在那个线程上执行
     * @return 是否需要通知唤醒线程
     */
    template <class FiberOrFunc>
    bool scheduleWithoutLock(FiberOrFunc f, bool s_thread = false,
                             pthread_t thread = 0) {
        // if (m_stopping) {
        //     LYON_LOG_WARN(LYON_LOG_GET_LOGGER("system")) << "Scheduler
        //     stoped!"; return false;
        // }
        bool tickle = m_jobs.empty();
        Job ft(f, s_thread, thread);
        if (ft.fiber || ft.cb) {
            m_jobs.push_back(ft);
        }
        return tickle;
    }

    /**
     * struct FiberAndFunc - 可被调度的单元，协程或者函数
     */
    struct Job {
        Fiber::ptr fiber = nullptr;
        std::function<void()> cb = nullptr;
        /**
         * @thread 指定运行的线程，只有在specified_thread==true是才有效
         */
        pthread_t thread;
        /**
         * @specified_thread 是否指定在哪个线程中执行
         */
        bool specified_thread = false;
        Job(Fiber::ptr f, bool s_thread = false, pthread_t thr = 0)
            : fiber(f), thread(thr), specified_thread(s_thread) {}

        Job(Fiber::ptr *f, bool s_thread = false, pthread_t thr = 0)
            : thread(thr), specified_thread(s_thread) {
            fiber.swap(*f);
        }

        Job(std::function<void()> f, bool s_thread = false, pthread_t thr = 0)
            : cb(f), thread(thr), specified_thread(s_thread) {}

        Job(std::function<void()> *f, bool s_thread = false, pthread_t thr = 0)
            : thread(thr), specified_thread(s_thread) {
            cb.swap(*f);
        }

        Job() : specified_thread(false) {}

        void reset() {
            fiber = nullptr;
            cb = nullptr;
            thread = 0;
            specified_thread = false;
        }
    };

protected:
    std::vector<pthread_t> m_threadIds;
    size_t m_threadCount = 0;
    std::atomic<size_t> m_activeThreadCount{0};
    std::atomic<size_t> m_idleThreadCount{0};
    bool m_stopping = true;
    bool m_autoStop = false;
    // int m_rootThread = 0;

private:
    MutexType m_mutex;
    /**
     * @m_threads 线程池
     */
    std::vector<Thread::ptr> m_threads;
    /**
     * @m_fibers 需要被调度的工作
     */
    std::list<Job> m_jobs;
    /**
     * @m_workFiber 表示当前调度器工作在哪个协程中
     */
    Fiber::ptr m_workFiber = nullptr;
    std::string m_name;
};

} // namespace lyon

#endif
