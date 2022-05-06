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

static thread_local Fiber *t_fiber = nullptr;
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

Fiber::Fiber(std::function<void()> cb, uint32_t stacksize)
    : m_id(s_fiber_id++), m_cb(cb) {
    s_fiber_count++;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getVal();
    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_context)) {
        LYON_ASSERT2(false, "get context");
    }

    m_context.uc_link = nullptr;
    m_context.uc_stack.ss_sp = m_stack;
    m_context.uc_stack.ss_size = m_stacksize;

    LYON_LOG_DEBUG(g_logger) << "Fiber::Fiber id = " << m_id;
}

void Fiber::reset(std::function<void()> cb) {}

void Fiber::swapIn() {}
void Fiber::swapOut() {}

void Fiber::SetThis(Fiber *f) { t_fiber = f; }
Fiber::ptr GetThis();

void Fiber::YieldToReady() {}
void Fiber::YieldToHold() {}

uint64_t Fiber::TotalFibers() { return s_fiber_count; }
} // namespace lyon
