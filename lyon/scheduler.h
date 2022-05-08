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

    template <class FiberOrCb>
    bool scheduleWithLock(FiberOrCb f, pthread_t thread) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleWithoutLock(f, thread);
        }
        if (need_tickle)
            tickle();
    }

    template <class InputIterator>
    bool scheduleWithLock(InputIterator begin, InputIterator end,
                          pthread_t thread) {
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
    static Scheduler *GetThis();

protected:
    virtual void tickle();

private:
    /**
     * @brief 添加调度任务
     *
     * @tparam FiberOrCb 任务类型
     * @param f 任务
     * @param thread 任务需要在那个线程上执行
     * @return 是否需要通知唤醒线程
     */
    template <class FiberOrCb>
    bool scheduleWithoutLock(FiberOrCb f, pthread_t thread) {
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
        long long thread;
        FiberAndFunc(Fiber::ptr f, int thr) : fiber(f), thread(thr) {}

        FiberAndFunc(Fiber::ptr *f, int thr) : thread(thr) { fiber.swap(*f); }

        FiberAndFunc(std::function<void()> f, int thr) : cb(f), thread(thr) {}

        FiberAndFunc(std::function<void()> *f, int thr) : thread(thr) {
            cb.swap(*f);
        }

        FiberAndFunc() : thread(-1) {}
    };

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
    std::string m_name;
};

} // namespace lyon

#endif
