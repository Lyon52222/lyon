#include "fiber.h"
#include "config.h"
#include "log.h"
#include <atomic>
#include <cstdint>
#include <macro.h>
#include <ucontext.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

static std::atomic<uint64_t> s_fiber_id{0};
static std::atomic<uint64_t> s_fiber_count{0};

/**
 * @brief 每个线程的主协程指针
 *
 */
static thread_local Fiber *t_fiber = nullptr;
/**
 * @brief 每个线程的主协程智能指针
 *
 */
static thread_local Fiber::ptr t_thread_fiber = nullptr;

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
    m_state = EXEC;
    SetThis(this);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }
    s_fiber_count++;
    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
}

Fiber::Fiber(std::function<void()> cb, uint32_t stacksize, bool use_caller)
    : m_id(s_fiber_id++), m_use_caller(use_caller), m_cb(cb) {
    s_fiber_count++;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getVal();
    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }

    m_context.uc_link = nullptr;
    m_context.uc_stack.ss_sp = m_stack;
    m_context.uc_stack.ss_size = m_stacksize;

    if (!use_caller)
        makecontext(&m_context, Fiber::MainFunc, 0);
    else
        makecontext(&m_context, Fiber::CallerMainFunc, 0);

    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber id = " << m_id;
}

Fiber::~Fiber() {
    s_fiber_count--;
    if (m_stack) {
        //只有在以下三种状态才允许析构
        LYON_ASSERT(m_state == INIT || m_state == TERM || m_state == EXCEPT);
        StackAllocator::Dealloc(m_stack, m_stacksize);
    } else {
        LYON_ASSERT(!m_cb);
        LYON_ASSERT(m_state == EXEC)

        Fiber *cur = t_fiber;
        if (cur == this) {
            SetThis(nullptr);
        }
    }
    LYON_LOG_DEBUG(g_logger)
        << "Fiber:~Fiber id = " << m_id << " total = " << s_fiber_count;
}

void Fiber::reset(std::function<void()> cb) {
    //必须保证协程栈已分配空间，且协程处于以下三种状态
    LYON_ASSERT(m_stack)
    LYON_ASSERT(m_state == INIT || m_state == TERM || m_state == EXCEPT);
    m_cb = cb;
}

void Fiber::swapIn() {}

void Fiber::swapOut() {}

void Fiber::MainFunc() {}

void Fiber::CallerMainFunc() {}

void Fiber::SetThis(Fiber *f) { t_fiber = f; }

Fiber::ptr Fiber::GetThis() {
    if (t_fiber) {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    LYON_ASSERT(t_fiber == main_fiber.get());
    t_thread_fiber = main_fiber;
    return t_fiber->shared_from_this();
}

void Fiber::YieldToReady() {}
void Fiber::YieldToHold() {}

uint64_t Fiber::TotalFibers() { return s_fiber_count; }
} // namespace lyon
