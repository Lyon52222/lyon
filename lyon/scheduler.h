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
 */
class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler(size_t threads = 1, bool use_caller = true,
              const std::string &name = "");

    virtual ~Scheduler();

    void start();
    void stop();
    void run();

    /**
     * @brief 加锁添加调度任务
     *
     * @tparam FiberOrCb 任务类型
     * @param f 调度任务
     * @param thread 任务在哪个线程上执行
     */
    template <class FiberOrCb> void scheduleWithLock(FiberOrCb f, int thread) {
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
    void scheduleWithLock(InputIterator begin, InputIterator end, int thread) {
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
    template <class FiberOrCb>
    bool scheduleWithoutLock(FiberOrCb f, int thread) {
        bool tickle = m_fibers.empty();
        FiberAndFunc ft(f, thread);
        if (ft.fiber || ft.cb) {
            m_fibers.push_back(ft);
        }
        return tickle;
    }

    /**
     * struct FiberAndFunc - 可被调度的单元，协程或者函数
     */
    struct FiberAndFunc {
        Fiber::ptr fiber = nullptr;
        std::function<void()> cb = nullptr;
        int thread;
        FiberAndFunc(Fiber::ptr f, int thr) : fiber(f), thread(thr) {}

        FiberAndFunc(Fiber::ptr *f, int thr) : thread(thr) { fiber.swap(*f); }

        FiberAndFunc(std::function<void()> f, int thr) : cb(f), thread(thr) {}

        FiberAndFunc(std::function<void()> *f, int thr) : thread(thr) {
            cb.swap(*f);
        }

        FiberAndFunc() : thread(-1) {}
    };

protected:
    std::vector<int> m_threadIds;
    size_t m_threadCount = 0;
    size_t m_activeThreadCount = 0;
    size_t m_idleThreadCount = 0;
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
    std::list<FiberAndFunc> m_fibers;
    Fiber::ptr m_rootFiber;
    std::string m_name;
};

} // namespace lyon

#endif
