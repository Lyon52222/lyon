#include "scheduler.h"
#include <log.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static thread_local Fiber *t_scheduler_fiber = nullptr;

Fiber *Scheduler::GetMainFiber() { return t_scheduler_fiber; }
} // namespace lyon
