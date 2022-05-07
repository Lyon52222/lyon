#include "fiber.h"
#include "config.h"
#include "log.h"
#include "macro.h"
#include "scheduler.h"
#include "util.h"
#include <atomic>
#include <cstdint>
#include <exception>
#include <string>
#include <ucontext.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

static std::atomic<uint64_t> s_fiber_id{0};
static std::atomic<uint64_t> s_fiber_count{0};

/**
 * @brief 表示的是当前线程正在运行的协程指针
 *
 */
static thread_local Fiber *t_current_fiber = nullptr;
/**
 * @brief 表示当前线程的主协程智能指针
 *
 */
static thread_local Fiber::ptr t_main_fiber = nullptr;

static ConfigVar<uint32_t>::ptr g_fiber_stack_size =
    Config::SetConfig<uint32_t>("fiber.stack_size", 1024 * 4,
                                "fiber stack size");

class MallocStackAllocator {
public:
    static void *Alloc(size_t size) { return malloc(size); }
    static void Dealloc(void *vp, size_t size) { free(vp); }
};

using StackAllocator = MallocStackAllocator;

Fiber::Fiber() {
    m_state = INIT;
    m_id = 0;
    SetCurentFiber(this);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }
    s_fiber_count++;
    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
}

Fiber::Fiber(std::function<void()> cb, uint32_t stacksize, bool use_caller)
    : m_id(++s_fiber_id), m_use_caller(use_caller), m_cb(cb) {
    m_state = INIT;
    s_fiber_count++;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getVal();
    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }

    Fiber::ptr cur = GetCurrentFiber();
    m_context.uc_link = nullptr;
    // m_context.uc_link = &cur->m_context;
    m_context.uc_stack.ss_sp = m_stack;
    m_context.uc_stack.ss_size = m_stacksize;

    if (!use_caller)
        makecontext(&m_context, Fiber::MainFunc, 0);
    else
        makecontext(&m_context, Fiber::CallerMainFunc, 0);

    m_state = READY;
    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber id = " << m_id;
}

Fiber::~Fiber() {
    s_fiber_count--;
    if (m_stack) {
        //只有在以下三种状态才允许析构
        LYON_ASSERT(m_state == INIT || m_state == READY || m_state == TERM ||
                    m_state == EXCEPT);
        StackAllocator::Dealloc(m_stack, m_stacksize);
    } else {
        LYON_ASSERT(!m_cb);
        LYON_ASSERT(m_state == INIT)

        Fiber *cur = t_current_fiber;
        if (cur == this) {
            SetCurentFiber(nullptr);
        }
    }
    LYON_LOG_DEBUG(g_logger)
        << "Fiber:~Fiber id = " << m_id << " total = " << s_fiber_count;
}

void Fiber::reset(std::function<void()> cb) {
    //必须保证协程栈已分配空间，且协程处于以下三种状态
    LYON_ASSERT(m_stack)
    LYON_ASSERT(m_state == READY || m_state == TERM || m_state == EXCEPT);
    m_cb = cb;

    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }

    m_context.uc_link = nullptr;
    m_context.uc_stack.ss_sp = m_stack;
    m_context.uc_stack.ss_size = m_stacksize;

    //使用MainFunc设置上下文，当m_context被载入时会运行MainFunc
    makecontext(&m_context, Fiber::MainFunc, 0);

    m_state = READY;
}

void Fiber::call() {
    SetCurentFiber(this);
    LYON_ASSERT(m_state != EXEC)
    m_state = EXEC;
    if (swapcontext(&Scheduler::GetMainFiber()->m_context, &m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::back() {
    SetCurentFiber(t_main_fiber.get());
    // m_state = HOLD;
    if (swapcontext(&m_context, &Scheduler::GetMainFiber()->m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::swapIn() {
    SetCurentFiber(this);
    LYON_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if (swapcontext(&t_main_fiber->m_context, &m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::swapOut() {
    SetCurentFiber(Scheduler::GetMainFiber());
    // m_state = HOLD;
    if (swapcontext(&m_context, &t_main_fiber->m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::YieldToReady() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = READY;
    cur->swapOut();
}

void Fiber::YieldToHold() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = HOLD;
    cur->swapOut();
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetCurrentFiber();
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } catch (std::exception &e) {
        cur->m_state = EXCEPT;
        LYON_LOG_ERROR(g_logger) << "Fiber Except: " << e.what()
                                 << " fiber id = " << cur->getId() << std::endl
                                 << BackTraceToString();
    } catch (...) {
        cur->m_state = EXCEPT;
        LYON_LOG_ERROR(g_logger) << "Fiber Except: "
                                 << " fiber id = " << cur->getId() << std::endl
                                 << BackTraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    //运行结束后切换为主协程
    // BUG:感觉这里不应该这样返回主协程，这样回将当前协程挂起，而不是结束,导致析构时报错
    raw_ptr->swapOut();
    LYON_ASSERT2(false, "Fiber never reached id = " +
                            std::to_string(raw_ptr->getId()));
}

void Fiber::CallerMainFunc() {
    Fiber::ptr cur = GetCurrentFiber();
    //在这里尝试运行cb函数
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } catch (std::exception &e) {
        cur->m_state = EXCEPT;
        LYON_LOG_ERROR(g_logger) << "Fiber Except: " << e.what()
                                 << " fiber id = " << cur->getId() << std::endl
                                 << BackTraceToString();
    } catch (...) {
        cur->m_state = EXCEPT;
        LYON_LOG_ERROR(g_logger) << "Fiber Except: "
                                 << " fiber id = " << cur->getId() << std::endl
                                 << BackTraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    //运行结束后切换回线程的主协程
    // BUG:感觉这里不应该这样返回主协程，这样回将当前协程挂起，而不是结束,导致析构时报错
    raw_ptr->back();
    LYON_ASSERT2(false, "Fiber never reached id = " +
                            std::to_string(raw_ptr->getId()));
}

void Fiber::SetCurentFiber(Fiber *f) { t_current_fiber = f; }

/**
 * @brief 获取当前的协程，当不存在协程时，创建主协程，并设置为当前协程
 *
 */
Fiber::ptr Fiber::GetCurrentFiber() {
    if (t_current_fiber) {
        return t_current_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    LYON_ASSERT(t_current_fiber == main_fiber.get());
    t_main_fiber = main_fiber;
    return t_current_fiber->shared_from_this();
}

uint64_t Fiber::TotalFibers() { return s_fiber_count; }
uint64_t Fiber::GetFiberId() {
    if (t_current_fiber) {
        return t_current_fiber->getId();
    }
    return 0;
}
} // namespace lyon
