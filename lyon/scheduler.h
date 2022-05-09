#ifndef __LYON_SCHEDULER_H__
#define __LYON_SCHEDULER_H__

#include "fiber.h"
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
     * @param use_caller 是否将创建调度器的线程加入线程池中
     * @param name 调度器名称
     */
    Scheduler(size_t threads = 1, bool use_caller = true,
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
     * @param thread 任务在哪个线程上执行
     */
    template <class FiberOrFunc>
    void scheduleWithLock(FiberOrFunc f, int thread = -1) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleWithoutLock(f, thread);
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
     * @param thread 任务在哪个线程上执行
     */
    template <class InputIterator>
    void scheduleWithLock(InputIterator begin, InputIterator end,
                          int thread = -1) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while (begin != end) {
                need_tickle =
                    scheduleWithoutLock(*begin, thread) || need_tickle;
                begin++;
            }
        }
        if (need_tickle)
            tickle();
    }

public:
    static Fiber *GetMainFiber();
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
    bool scheduleWithoutLock(FiberOrFunc f, int thread) {
        bool tickle = m_jobs.empty();
        Job ft(f, thread);
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
        int thread = -1;
        Job(Fiber::ptr f, int thr) : fiber(f), thread(thr) {}

        Job(Fiber::ptr *f, int thr) : thread(thr) { fiber.swap(*f); }

        Job(std::function<void()> f, int thr) : cb(f), thread(thr) {}

        Job(std::function<void()> *f, int thr) : thread(thr) { cb.swap(*f); }

        Job() : thread(-1) {}

        void reset() {
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }
    };

protected:
    std::vector<int> m_threadIds;
    size_t m_threadCount = 0;
    std::atomic<size_t> m_activeThreadCount{0};
    std::atomic<size_t> m_idleThreadCount{0};
    bool m_stopping = true;
    bool m_autoStop = false;
    int m_rootThread = 0;

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
    Fiber::ptr m_rootFiber = nullptr;
    std::string m_name;
};

} // namespace lyon

#endif
