#ifndef __LYON__THREAD_H__
#define __LYON__THREAD_H__
#include "log.h"
#include "util.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <pthread.h>
#include <thread>

namespace lyon {

class Thread : boost::noncopyable {
  public:
    typedef std::shared_ptr<Thread> ptr;
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
};

} // namespace lyon
#endif // !__LYON__THREAD_H__
