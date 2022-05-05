#include "mutex.h"
#include <stdexcept>
namespace lyon {

Semaphore::Semaphore(uint32_t count) {
    // INFO: macos 不支持创建无名信号量
    if (sem_init(&m_semaphore, 0, count)) {
        throw std::logic_error("sem_init error");
    }
}
Semaphore::~Semaphore() {}
void Semaphore::wait() {}
void Semaphore::notify() {}

} // namespace lyon
