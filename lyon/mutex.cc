#include "mutex.h"
#include <stdexcept>
namespace lyon {

NamedSemaphore::NamedSemaphore(const std::string &name, uint32_t count)
    : m_name(name) {
    // NOTE: macos 不支持创建无名信号量
    if ((m_semaphore = sem_open(m_name.c_str(), O_CREAT | O_EXCL, S_IRWXU,
                                count)) == SEM_FAILED) {
        throw std::logic_error("sem_open error");
    }
}

NamedSemaphore::~NamedSemaphore() {
    sem_close(m_semaphore);
    sem_unlink(m_name.c_str());
}
void NamedSemaphore::wait() {
    if (sem_wait(m_semaphore)) {
        throw std::logic_error("sem_wait error");
    }
}
void NamedSemaphore::notify() {
    if (sem_post(m_semaphore)) {
        throw std::logic_error("sem_post error");
    }
}

} // namespace lyon
