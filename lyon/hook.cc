#include "hook.h"
#include "iomanager.h"
#include "log.h"
#include <dlfcn.h>

namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static thread_local bool s_hook_enable = false;

#define HOOK_FUN(XX) XX(sleep)

void hook_init() {
//将name_f 指向原函数
#define XX(name) name##_f = (name##_fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX)
#undef XX
}

struct _HookIniter {
    _HookIniter() { hook_init(); }
};

static _HookIniter s_hook_initer;

bool is_hook_enable() { return s_hook_enable; }

void set_hook_enable(bool flag) { s_hook_enable = flag; }

extern "C" {

#define XX(name) name##_fun name##_f = nullptr;
HOOK_FUN(XX)
#undef XX

unsigned int sleep(unsigned int seconds) {
    if (!s_hook_enable) {
        return sleep_f(seconds);
    }
    IOManager *iom = IOManager::GetCurrentIOManager();
    Fiber::ptr fiber = Fiber::GetCurrentFiber();

    //通过设置一个定时器事件来唤醒
    // iom->addTimer(seconds * 1000,
    //               std::bind((void(Scheduler::*)(Fiber::ptr, bool s_thread,
    //                                             int
    //                                             thread))IOManager::addJob,
    //                         iom, fiber, false, 0));
    iom->addTimer(seconds * 1000, [&iom, &fiber]() { iom->addJob(fiber); });

    Fiber::HoldToScheduler();
    return 0;
}
}

} // namespace lyon
