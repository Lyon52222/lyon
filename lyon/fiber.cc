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

static std::atomic<uint64_t> s_fiber_id{1};
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
    //线程主协程的id是1, 后续协程编号从2开始
    m_id = 1;
    SetCurrentFiber(this);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }
    s_fiber_count++;
    m_is_main = true;
    m_state = EXEC;
    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber main id = " << m_id
                             << " fiber_count = " << s_fiber_count;
}

Fiber::Fiber(std::function<void()> cb, bool back_scheduler, bool create_main,
             uint32_t stacksize)
    : m_id(++s_fiber_id), m_cb(cb) {
    m_state = INIT;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getVal();
    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }

    //这一步是为了探测主协程是否存在，不存在就创建主协程
    //不过如果是以调度器协程为主协程就不需要创建了
    if (create_main)
        Fiber::ptr cur = GetMainFiber();

    m_context.uc_link = nullptr;
    // INFO: 通过这种方法也可以使得，协程结束后返回主协程，
    // 如果没有返回主协程的话，该协程结束后，程序会直接结束
    //  m_context.uc_link = &t_main_fiber->m_context;
    m_context.uc_stack.ss_sp = m_stack;
    m_context.uc_stack.ss_size = m_stacksize;

    if (back_scheduler) {
        makecontext(&m_context, Fiber::SchedulerFunc, 0);
    } else {
        makecontext(&m_context, Fiber::MainFiberFunc, 0);
    }
    m_state = READY;
    s_fiber_count++;
    LYON_LOG_DEBUG(g_logger)
        << "Fiber::Fiber id = " << m_id << " fiber_count = " << s_fiber_count;
}

Fiber::~Fiber() {
    if (m_stack) {
        //只有在以下三种状态才允许析构
        LYON_ASSERT(m_state == INIT || m_state == READY || m_state == TERM ||
                    m_state == EXCEPT);
        StackAllocator::Dealloc(m_stack, m_stacksize);
    } else {
        //这种情况可能是初始化未完成，也可能是，该协程是主协程
        LYON_ASSERT(!m_cb);
        LYON_ASSERT(m_is_main || m_state == INIT)

        Fiber *cur = t_current_fiber;
        if (cur == this) {
            SetCurrentFiber(nullptr);
        }
    }

    s_fiber_count--;
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

    makecontext(&m_context, Fiber::SchedulerFunc, 0);

    m_state = READY;
}

void Fiber::mainFiberIn() {
    SetCurrentFiber(this);
    LYON_ASSERT(m_state != EXEC)
    m_state = EXEC;
    if (swapcontext(&t_main_fiber->m_context, &m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::mainFiberOut() {
    SetCurrentFiber(t_main_fiber.get());
    // m_state = HOLD;
    if (swapcontext(&m_context, &t_main_fiber->m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::schedulerIn() {
    SetCurrentFiber(this);
    LYON_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if (swapcontext(&Scheduler::GetMainFiber()->m_context, &m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::schedulerOut() {
    SetCurrentFiber(Scheduler::GetMainFiber());
    // m_state = HOLD;
    if (swapcontext(&m_context, &Scheduler::GetMainFiber()->m_context)) {
        LYON_ASSERT2(false, "swapcontext");
    }
}

void Fiber::ReadyToScheduler() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = READY;
    cur->schedulerOut();
}

void Fiber::HoldToScheduler() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = HOLD;
    cur->schedulerOut();
}

void Fiber::ReadyToMainFiber() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = READY;
    cur->mainFiberOut();
}

void Fiber::HoldToMainFiber() {
    Fiber::ptr cur = GetCurrentFiber();
    LYON_ASSERT(cur->m_state == EXEC);
    cur->m_state = HOLD;
    cur->mainFiberOut();
}

void Fiber::SchedulerFunc() {
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
    raw_ptr->schedulerOut();
    LYON_ASSERT2(false, "Fiber never reached id = " +
                            std::to_string(raw_ptr->getId()));
}

void Fiber::MainFiberFunc() {
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
    raw_ptr->mainFiberOut();
    LYON_ASSERT2(false, "Fiber never reached id = " +
                            std::to_string(raw_ptr->getId()));
}

void Fiber::SetCurrentFiber(Fiber *f) { t_current_fiber = f; }

void Fiber::SetMainFiber(Fiber::ptr f) { t_main_fiber = f; }
/**
 * @brief 获取当前线程正在运行的协程
 * 如果当前线程已经有协程在运行，就返回正在运行的协程。
 * 否则，为当前线程创建协程并且返回
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

Fiber::ptr Fiber::GetMainFiber() {
    if (!t_main_fiber) {
        t_main_fiber.reset(new Fiber);
    }
    return t_main_fiber;
}
uint64_t Fiber::TotalFibers() { return s_fiber_count; }
uint64_t Fiber::GetFiberId() {
    if (t_current_fiber) {
        return t_current_fiber->getId();
    }
    return 0;
}
} // namespace lyon
