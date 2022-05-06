#ifndef __LYON__THREAD_H__
#define __LYON__THREAD_H__
#include "mutex.h"
#include "util.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <pthread.h>
#include <string>
#include <thread>

namespace lyon {

/**
 * @brief pthread的封装类
 */
class Thread : boost::noncopyable {
public:
    typedef std::shared_ptr<Thread> ptr;

    /**
     * @brief 创建一个运行cb函数的名为name的线程
     *
     * @param cb 线程函数
     * @param name 线程名
     */
    Thread(std::function<void()> cb, const std::string &name);

    ~Thread();

    pid_t getId() const { return m_id; };

    const std::string &getName() const { return m_name; };

    static std::string &GetName();

    static void SetName(const std::string &name);

    static Thread *GetThis();

    void join();

private:
    static void *run(void *arg);

    std::string m_name;
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb = nullptr;
    Semaphore m_semaphore;
};

} // namespace lyon
#endif // !__LYON__THREAD_H__
