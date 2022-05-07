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
    Fiber();

public:
    typedef std::shared_ptr<Fiber> ptr;
    /**
     * @{name} 协程状态
     */
    enum State {
        //初始化
        INIT,
        //暂停
        HOLD,
        //执行
        EXEC,
        //结束
        TERM,
        //就绪
        READY,
        //异常
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

    /**
     * @brief 重置协程调用函数
     *
     * @param cb 协程调用函数
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 切换到当前协程
     *
     */
    void swapIn();

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
     * @brief 设置当前协程
     *
     * @param f 协程指针
     */
    static void SetThis(Fiber *f);

    /**
     * @brief 获取当前协程
     *
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 协程切换到后台并且设置为ready状态
     *
     */
    static void YieldToReady();

    /**
     * @brief 协程切换到后台并且设置为hold状态
     *
     */
    static void YieldToHold();

    static uint64_t TotalFibers();

private:
    uint64_t m_id;
    uint32_t m_stacksize = 0;
    bool m_use_caller = false;
    State m_state = INIT;
    ucontext_t m_context;
    void *m_stack = nullptr;
    std::function<void()> m_cb = nullptr;
};
} // namespace lyon

#endif // !__LYON_FIBER_H__
