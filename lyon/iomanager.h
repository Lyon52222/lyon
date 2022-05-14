#ifndef __LYON_IOMANAGER_H__
#define __LYON_IOMANAGER_H__

#include "mutex.h"
#include "scheduler.h"
#include "time.h"
#include "timer.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <sys/epoll.h>
namespace lyon {

class IOManager : public Scheduler, public TimerManager {
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
    bool deleEvent(int fd, Event event);
    bool cancelEvent(int fd, Event event);
    bool cancelAll(int fd);

public:
    static IOManager *GetCurrentIOManager();

protected:
    /**
     * @brief 重载的虚函数，当调度器没有其他任务执行时，运行此函数。
     * 在这里用于查询epoll，并且执行其所对应的EventContext的cb或fiber
     *
     */
    void idle() override;
    /**
     * @brief 重载函数，用于向其它线程发送信号
     *
     */
    void tickle() override;

    /**
     * @brief 判断调度任务是否已经结束
     *
     * @return 调度是否结束
     */
    bool stopping() override;

    bool stopping(uint64_t &next_timeout);

    void onTimerInsertAtFront() override;

private:
    /**
     * struct FdContext - 对应每个文件描述符的描述类
     */
    struct FdContext {
        typedef Mutex MutexType;
        /**
         * struct EventContext - 每个描述符对应事件的描述类
         */
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
         * @brief 获取事件所对应的EventContext
         *
         * @param e 事件e
         */
        EventContext &getEventContext(Event e);

        /**
         * @brief 重置EventContext
         *
         * @param ctx 事件上下文
         */
        void resetEventContext(EventContext &ctx);

        /**
         * @brief 触发事件e所对应的事件描述符
         *
         * @param e 事件e
         */
        void triggerEvent(Event e);

        /**
         * @fd 事件关联的句柄
         */
        int fd = 0;
        /**
         * @read 文件描述符对应读事件的描述上下文
         */
        EventContext read;
        /**
         * @write 文件描述符对应写事件的描述上下文
         */
        EventContext write;
        //该文件描述符所关注的事件
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
