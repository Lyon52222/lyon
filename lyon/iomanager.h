#ifndef __LYON_IOMANAGER_H__
#define __LYON_IOMANAGER_H__

#include "mutex.h"
#include "scheduler.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <sys/epoll.h>
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
        NONE = 0x000,
        //读事件 (EPOLLIN)
        READ = EPOLLIN,
        //写事件 (EPOLLOUT)
        WRITE = EPOLLOUT
    };

    IOManager(size_t threads = 1, bool join_fiber = true,
              const std::string &name = "");

    ~IOManager();

    void resizeContext(size_t size);

    /**
     * @brief 向epfd中添加监听事件
     *
     * @param fd 事件对应的Fd
     * @param event 需要添加的事件
     * @param cb 回调函数
     * @return 是否添加成功
     */
    int addEvent(int fd, Event event, std::function<void()> cb = nullptr);
    /**
     * @brief 删除epfd上对应的事件
     *
     * @param fd fd事件对应的Fd
     * @param event 需要删除的事件
     * @return 是否删除成功
     */
    int deleEvent(int fd, Event event);
    bool cancleEvent(int fd, Event event);

public:
    static IOManager *GetCurrentIOManager();

protected:
    /**
     * @brief 重载的虚函数， 当调度器没有其他任务执行时，运行此函数。
     * 在这里用于查询epoll
     *
     */
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

        EventContext &getEventContext(Event e);

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
        Event events = NONE;
        MutexType mutex;
    };

private:
    int m_epfd;
    int m_tickleFds[2];
    std::atomic<size_t> m_penddingEventCount{0};
    std::vector<FdContext *> m_fdContexts;
    RWMutexType m_mutex;
};

} // namespace lyon

#endif // !__LYON_IOMANAGER_H__
