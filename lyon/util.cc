#include "util.h"
#include <sys/syscall.h>

namespace lyon {
pid_t GetThreadId() { return 0; }
uint32_t GetFiberId() { return 0; }
static const std::string &GetThreadName() {
    static std::string name = "todo";
    return name;
}
} // namespace lyon
