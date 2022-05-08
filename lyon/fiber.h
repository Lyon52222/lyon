#ifndef __LYON_FIBER_H__
#define __LYON_FIBER_H__

#include <boost/core/noncopyable.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <sys/ucontext.h>
#include <ucontext.h>

namespace lyon {

class Fiber : public std::enable_shared_from_this<Fiber> {
private:
    /**
     * @brief 主协程的构造函数
     *
     */
    Fiber();

public:
    typedef std::shared_ptr<Fiber> ptr;
    /**
     * @{name} 协程状态
     */
    enum State {
        //初始化, 协程正在初始化中，不保证stack已经分配
        INIT,
        //就绪，协程已经完成初始化工作，可以随时调度
        READY,
        //暂停，协程被暂停，资源依然保存，可随时运行
        HOLD,
        //执行，正在执行中
        EXEC,
        //结束，协程正常结束
        TERM,
        //异常，协程非正常结束
        EXCEPT
    };

    /**
     * @brief 协程构造函数
     *
     * @param cb 协程执行函数
     * @param stacksize 协程栈大小
     * @param use_caller 是否在Mainfiber上调度
     */
    Fiber(std::function<void()> cb, uint32_t stacksize = 0,
          bool use_caller = false);
    ~Fiber();

    uint64_t getId() { return m_id; }
    State getState() { return m_state; }

    /**
     * @brief 重置协程调用函数,用于重复利用协程的栈空间
     *
     * @param cb 协程调用函数
     */
    void reset(std::function<void()> cb);

    void call();

    void back();

    /**
     * @brief 切换到当前协程
     *
     */
    void swapIn();

private:
    /**
     * @brief 将当前协程切换到后台
     *
     */
    void swapOut();

public:
    /**
     * @brief 协程执行函数，完成后返回主协程
     *
     */
    static void MainFunc();

    /**
     * @brief 协程执行函数，完成后返回调用协程
     *
     */
    static void CallerMainFunc();

    /**
     * @brief 设置当前正在运行的协程
     *
     * @param f 协程指针
     */
    static void SetCurentFiber(Fiber *f);

    /**
     * @brief 获取当前正在运行的协程
     *
     */
    static Fiber::ptr GetCurrentFiber();

    /**
     * @brief 将当前正在运行的协程切换到后台并且设置为ready状态
     *
     */
    static void YieldToReady();

    /**
     * @brief 将当前正在运行的协程切换到后台并且设置为hold状态
     *
     */
    static void YieldToHold();

    /**
     * @brief 获取总协程数
     *
     * @return 当前线程中的协程数
     */
    static uint64_t TotalFibers();

    /**
     * @brief 获取当前协程Id
     *
     * @return 协程Id
     */
    static uint64_t GetFiberId();

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    bool m_use_caller = false;
    State m_state = INIT;
    ucontext_t m_context;
    void *m_stack = nullptr;
    std::function<void()> m_cb = nullptr;
};
} // namespace lyon

#endif // !__LYON_FIBER_H__
