#ifndef __LYON_MUTEX_H__
#define __LYON_MUTEX_H__
#include <atomic>
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <pthread.h>
#include <semaphore.h>
#include <string>

namespace lyon {
class Semaphore : boost::noncopyable {
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();

private:
    sem_t m_semaphore;
};

/**
 * struct ScopeLockImpl - 局部锁封装，当离开锁的作用域时，析构函数会自动解锁
 *
 * @tparam T 锁类型
 */
template <class T> struct ScopeLockImpl {
public:
    ScopeLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
    }

    void lock() {
        if (!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

    ~ScopeLockImpl() { unlock(); }

private:
    T &m_mutex;
    bool m_locked = false;
};

/**
 * struct ReadScopeLockImpl - 读局部锁封装
 *
 * @tparam T 锁类型
 */
template <class T> struct ReadScopeLockImpl {
public:
    ReadScopeLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.rdlock();
        m_locked = true;
    }

    void lock() {
        if (!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

    ~ReadScopeLockImpl() { unlock(); }

private:
    T &m_mutex;
    bool m_locked = false;
};

/**
 * struct WriteScopeLockImpl - 写局部锁封装
 *
 * @tparam T 锁类型
 */
template <class T> struct WriteScopeLockImpl {
public:
    WriteScopeLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.wrlock();
        m_locked = true;
    }

    void lock() {
        if (!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

    ~WriteScopeLockImpl() { unlock(); }

private:
    T &m_mutex;
    bool m_locked = false;
};

/**
 * @brief 互斥量，等待的线程会陷入休眠，涉及到系统态的上下文切换。
 * 常用于需要长时间等待的互斥操作
 */
class Mutex : boost::noncopyable {
public:
    typedef ScopeLockImpl<Mutex> Lock;
    Mutex() { pthread_mutex_init(&m_mutex, nullptr); }

    void lock() { pthread_mutex_lock(&m_mutex); }

    void unlock() { pthread_mutex_unlock(&m_mutex); }

    ~Mutex() { pthread_mutex_destroy(&m_mutex); }

private:
    pthread_mutex_t m_mutex;
};

/**
 * @brief 读写锁，读写分离。读读不互斥。
 * 常用于读多写少的情况
 */
class RWMutex : boost::noncopyable {
public:
    typedef ReadScopeLockImpl<RWMutex> RDLock;
    typedef WriteScopeLockImpl<RWMutex> WRLock;

    RWMutex() { pthread_rwlock_init(&m_lock, nullptr); }

    void rdlock() { pthread_rwlock_rdlock(&m_lock); }

    void wrlock() { pthread_rwlock_wrlock(&m_lock); }

    void unlock() { pthread_rwlock_unlock(&m_lock); }

    ~RWMutex() { pthread_rwlock_destroy(&m_lock); }

private:
    pthread_rwlock_t m_lock;
};

/**
 * @brief
 * 自旋锁，系统实现的自旋锁，一般是先自旋等待一段时间，然后在休眠。自选等待不休眠，占用CPU。
 * 常用于短时间占用的锁
 */
class SpinLock : boost::noncopyable {
public:
    typedef ScopeLockImpl<SpinLock> Lock;
    SpinLock() { pthread_spin_init(&m_mutex, 0); }

    void lock() { pthread_spin_lock(&m_mutex); }

    void unlock() { pthread_spin_unlock(&m_mutex); }

    ~SpinLock() { pthread_spin_destroy(&m_mutex); }

private:
    pthread_spinlock_t m_mutex;
};

/**
 * @brief 自己通过CAS操作实现的自旋转锁，一只占用CPU
 */
class CASLock : boost::noncopyable {
public:
    typedef ScopeLockImpl<CASLock> Lock;
    CASLock() { m_mutex.clear(); }
    ~CASLock() {}

    void lock() {
        while (std::atomic_flag_test_and_set_explicit(
            &m_mutex, std::memory_order_acquire))
            ;
    }

    void unlock() {
        std::atomic_flag_test_and_set_explicit(&m_mutex,
                                               std::memory_order_release);
    }

private:
    volatile std::atomic_flag m_mutex;
};

} // namespace lyon

#endif // !__LYON_MUTEX_H__
