#include "util.h"
#include "fiber.h"
#include "log.h"
#include <cstdlib>
#include <execinfo.h>
#include <pthread.h>
#include <sstream>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

uint64_t GetCurrentThreadId() {
    uint64_t id;
    id = syscall(SYS_gettid);
    return id;
}

uint32_t GetCurrentFiberId() { return Fiber::GetFiberId(); }

void BackTrace(std::vector<std::string> &bt, int size, int skip) {
    void **array = (void **)malloc(sizeof(void *) * size);
    int s = backtrace(array, size);
    char **strings = backtrace_symbols(array, s);

    if (strings == nullptr) {
        LYON_LOG_ERROR(g_logger) << "backtrace fail";
        return;
    }
    for (int i = skip; i < s; i++) {
        bt.push_back(strings[i]);
    }

    free(array);
    free(strings);
}

std::string BackTraceToString(int size, int skip, const std::string &prefix) {
    std::vector<std::string> bt;
    BackTrace(bt, size, skip);
    std::stringstream ss;
    for (auto &i : bt) {
        ss << prefix << i << std::endl;
    }
    return ss.str();
}

bool IsConfigNameAvilable(const std::string &name) {
    return name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ"
                                  "RSTUVWXYZ_.0123456789") == std::string::npos;
}

} // namespace lyon
