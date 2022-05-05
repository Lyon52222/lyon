#ifndef __LYON_MUTEX_H__
#define __LYON_MUTEX_H__
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <semaphore.h>

namespace lyon {
class Semaphore : boost::noncopyable {
  public:
    Semaphore(uint32_t count);
    ~Semaphore();
    void wait();
    void notify();

  private:
    sem_t m_semaphore;
};

} // namespace lyon

#endif // !__LYON_MUTEX_H__
