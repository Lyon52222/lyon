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
    Semaphore(uint32_t count);
    ~Semaphore();
    void wait();
    void notify();

private:
    sem_t m_semaphore;
};

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

class RWMutex : boost::noncopyable {
public:
    typedef ReadScopeLockImpl<RWMutex> RDLock;
    typedef WriteScopeLockImpl<RWMutex> WRLock;

    RWMutex() { pthread_rwlock_init(&m_mutex, nullptr); }

    void rdlock() { pthread_rwlock_rdlock(&m_mutex); }

    void wrlock() { pthread_rwlock_wrlock(&m_mutex); }

    void unlock() { pthread_rwlock_unlock(&m_mutex); }

    ~RWMutex() { pthread_rwlock_destroy(&m_mutex); }

private:
    pthread_rwlock_t m_mutex;
};

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
