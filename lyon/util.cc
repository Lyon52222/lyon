#include "util.h"
#include <pthread.h>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

namespace lyon {
/**
 * @brief 获取线程ID
 *
 * @return 线程的ID
 */
uint64_t GetThreadId() {
    uint64_t id;
    id = syscall(SYS_gettid);
    return id;
}
uint32_t GetFiberId() { return 0; }

} // namespace lyon
