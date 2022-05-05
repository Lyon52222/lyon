#include "util.h"
#include <pthread.h>
#include <thread>

namespace lyon {
/**
 * @brief 获取线程ID
 *
 * @return 线程的ID
 */
uint64_t GetThreadId() {
    uint64_t id;
    // NOTE: syscall 在macos10. 后被弃用了。
    pthread_threadid_np(0, &id);
    return id;
}
uint32_t GetFiberId() { return 0; }

} // namespace lyon
