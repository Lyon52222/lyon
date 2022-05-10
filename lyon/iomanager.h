#ifndef __LYON_IOMANAGER_H__
#define __LYON_IOMANAGER_H__

#include "mutex.h"
#include "scheduler.h"
#include <functional>
#include <memory>
namespace lyon {

class IOManager : public Scheduler {
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef RWMutex RWMutexType;
    /**
     * @{name} io事件
     */
    enum Event {
        //无事件
        NONE = 0x01,
        //读事件
        READ = 0x02,
        //写事件
        WRITE = 0x03
    };

    IOManager(size_t threads = 1, bool join_fiber = true,
              const std::string &name = "");

    ~IOManager();

protected:
    void idle() override;
    void tickle() override;

private:
    struct FdContext {
        typedef Mutex MutexType;
        struct EventContext {
            /**
             * @{name} 事件执行的调度器
             */
            Scheduler *scheduler = nullptr;
            /**
             * @fiber 事件执行的协程
             */
            Fiber::ptr fiber;
            /**
             * @cb 事件回调用函数
             */
            std::function<void()> cb;
        };

        /**
         * @fd 事件关联的句柄
         */
        int fd = 0;
        /**
         * @read 读事件上下文
         */
        EventContext read;
        /**
         * @write 写事件上下文
         */
        EventContext write;
        Event event = NONE;
        MutexType mutex;
    };

private:
    int m_epfd;
    RWMutexType m_mutex;
};

} // namespace lyon

#endif // !__LYON_IOMANAGER_H__
