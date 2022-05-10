#include "iomanager.h"
#include <log.h>
#include <scheduler.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
namespace lyon {

IOManager::IOManager(size_t threads, bool join_fiber, const std::string &name)
    : Scheduler(threads, join_fiber, name) {}

IOManager::~IOManager() {}

void IOManager::idle() {}

void IOManager::tickle() {}

} // namespace lyon
