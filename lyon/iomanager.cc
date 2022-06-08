#include "iomanager.h"
#include "log.h"
#include "macro.h"
#include "scheduler.h"
#include <asm-generic/errno-base.h>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fcntl.h>
#include <functional>
#include <stdexcept>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace lyon {
static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

//格式化输出events对应的文本。 这种方法相比于getString很有用
static std::ostream &operator<<(std::ostream &os, EPOLL_EVENTS events) {
    if (!events)
        return os << "NONE";
    bool first = true;
#define XX(E)                                                                  \
    if (E & events) {                                                          \
        if (!first) {                                                          \
            os << '|';                                                         \
        }                                                                      \
        os << #E;                                                              \
        first = false;                                                         \
    }
    XX(EPOLLIN);
    XX(EPOLLPRI);
    XX(EPOLLOUT);
    XX(EPOLLRDNORM);
    XX(EPOLLRDBAND);
    XX(EPOLLWRNORM);
    XX(EPOLLWRBAND);
    XX(EPOLLMSG);
    XX(EPOLLERR);
    XX(EPOLLHUP);
    XX(EPOLLRDHUP);
    XX(EPOLLEXCLUSIVE);
    XX(EPOLLWAKEUP);
    XX(EPOLLONESHOT);
    XX(EPOLLET);
#undef XX
    return os;
} // namespace lyon

IOManager::IOManager(size_t threads, bool join_fiber, const std::string &name)
    : Scheduler(threads, join_fiber, name) {
    m_epfd = epoll_create(5000);
    LYON_ASSERT(m_epfd != -1);

    //创建管道，0用于输入，1用于输出
    int rt = pipe(m_tickleFds);
    LYON_ASSERT(!rt);

    epoll_event event;
    memset(&event, 0, sizeof(event));
    //关注的事件为有数据可读和边缘触发
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = m_tickleFds[0];

    //设置文件描述符为非阻塞IO
    rt = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
    LYON_ASSERT(rt != -1);

    //用于添加，修改或删除epoll描述符上的事件
    rt = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
    LYON_ASSERT(!rt);

    resizeContext(32);
    //开始调度
    start();
}

IOManager::~IOManager() {
    //停止调度
    stop();
    //关闭文件描述符

    close(m_epfd);
    close(m_tickleFds[0]);
    close(m_tickleFds[1]);

    for (size_t i = 0; i < m_fdContexts.size(); i++) {
        if (m_fdContexts[i])
            delete m_fdContexts[i];
    }
}

void IOManager::resizeContext(size_t size) {
    m_fdContexts.resize(size);
    for (size_t i = 0; i < m_fdContexts.size(); i++) {
        if (!m_fdContexts[i]) {
            m_fdContexts[i] = new FdContext;
            m_fdContexts[i]->fd = i;
        }
    }
}

int IOManager::addEvent(int fd, Event event, std::function<void()> cb) {
    FdContext *fd_ctx = nullptr;
    RWMutexType::RDLock rlock(m_mutex);
    if (fd < static_cast<int>(m_fdContexts.size())) {
        fd_ctx = m_fdContexts[fd];
        rlock.unlock();
    } else {
        rlock.unlock();
        RWMutexType::WRLock wlock(m_mutex);
        resizeContext(fd * 1.5);
        fd_ctx = m_fdContexts[fd];
    }

    FdContext::MutexType::Lock lock(fd_ctx->mutex);

    //如果需要添加的事件已经存在，就直接返回
    if (LYON_UNLIKELY(fd_ctx->events & event)) {
        LYON_LOG_WARN(g_logger)
            << "IOManager: event exist - event = " << event
            << " fd_ctx.events = " << (EPOLL_EVENTS)fd_ctx->events;
        return -1;
    }

    int op = fd_ctx->events ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    // Event new_event = static_cast<Event>(fd_ctx->events | event);

    epoll_event epevent;
    epevent.events = static_cast<Event>(EPOLLET) | fd_ctx->events | event;
    //设置事件对应的附加 数据指针，后续可以重新得到。
    epevent.data.ptr = fd_ctx;

    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        // if (errno == EEXIST) {
        // } else {
        LYON_LOG_ERROR(g_logger)
            << "IOManager: "
            << "epoll_ctl( " << m_epfd << ", " << op << ", " << fd << ", "
            << (EPOLL_EVENTS)epevent.events << " ): " << rt
            << " ( errno: " << errno << "-" << strerror(errno)
            << " ) fd_ctx.events = " << (EPOLL_EVENTS)fd_ctx->events;
        return -1;
        // }
    }
    m_penddingEventCount++;

    // fd_ctx->events = new_event;
    fd_ctx->events = static_cast<Event>(fd_ctx->events | event);

    //获取对应事件的事件描述上下文
    auto &event_ctx = fd_ctx->getEventContext(event);

    LYON_LOG_INFO(g_logger)
        << "addevent fd = " << fd << " event = " << (EPOLL_EVENTS)event;

    LYON_ASSERT(!event_ctx.cb && !event_ctx.fiber && !event_ctx.scheduler);

    event_ctx.scheduler = GetCurrentScheduler();
    if (cb) {
        event_ctx.cb.swap(cb);
    } else {
        //如果未设置对应的处理函数
        event_ctx.fiber = Fiber::GetCurrentFiber();
        LYON_ASSERT2(event_ctx.fiber->getState() == Fiber::EXEC,
                     "Fiber state error = " << event_ctx.fiber->getState());
    }

    return 0;
}

bool IOManager::deleEvent(int fd, Event event) {
    LYON_LOG_INFO(g_logger)
        << "deleevent fd = " << fd << " event = " << (EPOLL_EVENTS)event;
    RWMutexType::RDLock rlock(m_mutex);
    FdContext *fd_ctx = nullptr;
    if (fd >= static_cast<int>(m_fdContexts.size())) {
        return false;
    } else {
        fd_ctx = m_fdContexts[fd];
        rlock.unlock();
    }
    FdContext::MutexType::Lock lock(fd_ctx->mutex);

    if (LYON_UNLIKELY(!(fd_ctx->events & event))) {
        LYON_LOG_WARN(g_logger)
            << "IOManager: event not exist - event = " << event
            << " fd_ctx.events = " << fd_ctx->events;
        return false;
    }

    Event new_event = static_cast<Event>(fd_ctx->events & (~event));

    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events = static_cast<Event>(EPOLLET) | new_event;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "IOManager: "
            << "epoll_ctl( " << m_epfd << ", " << op << ", " << fd << ", "
            << (EPOLL_EVENTS)epevent.events << " ): " << rt << " ( " << errno
            << "-" << strerror(errno)
            << " ) fd_ctx = " << (EPOLL_EVENTS)fd_ctx->events;
        return false;
    }
    m_penddingEventCount--;
    fd_ctx->events = new_event;
    auto &event_ctx = fd_ctx->getEventContext(event);
    fd_ctx->resetEventContext(event_ctx);
    return true;
}

bool IOManager::triggerEvent(int fd, Event event) {
    LYON_LOG_INFO(g_logger)
        << "triggerevent fd = " << fd << " event = " << (EPOLL_EVENTS)event;
    RWMutexType::RDLock rlock(m_mutex);
    FdContext *fd_ctx = nullptr;
    if (fd >= static_cast<int>(m_fdContexts.size())) {
        return false;
    } else {
        fd_ctx = m_fdContexts[fd];
        rlock.unlock();
    }
    FdContext::MutexType::Lock lock(fd_ctx->mutex);

    if (LYON_UNLIKELY(!(fd_ctx->events & event))) {
        LYON_LOG_WARN(g_logger)
            << "IOManager: event not exist - event = " << (EPOLL_EVENTS)event
            << " fd_ctx.events = " << (EPOLL_EVENTS)fd_ctx->events;
        return false;
    }

    Event new_event = static_cast<Event>(fd_ctx->events & (~event));

    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events = static_cast<Event>(EPOLLET) | new_event;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "IOManager: "
            << "epoll_ctl( " << m_epfd << ", " << op << ", " << fd << ", "
            << (EPOLL_EVENTS)epevent.events << " ): " << rt << " ( " << errno
            << "-" << strerror(errno)
            << " ) fd_ctx = " << (EPOLL_EVENTS)fd_ctx->events;
        return false;
    }

    fd_ctx->triggerEvent(event);
    m_penddingEventCount--;
    return true;
}

bool IOManager::triggerAll(int fd) {
    RWMutexType::RDLock rlock(m_mutex);
    FdContext *fd_ctx = nullptr;
    if (fd >= static_cast<int>(m_fdContexts.size())) {
        return false;
    } else {
        fd_ctx = m_fdContexts[fd];
        rlock.unlock();
    }
    FdContext::MutexType::Lock lock(fd_ctx->mutex);

    // if (LYON_UNLIKELY(!(fd_ctx->events))) {
    //     LYON_LOG_WARN(g_logger)
    //         << "IOManager: event not exist "
    //         << " fd_ctx.events = " << (EPOLL_EVENTS)fd_ctx->events;
    //     return false;
    // }
    if (!fd_ctx->events) {
        return false;
    }

    int op = EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = 0;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "IOManager: "
            << "epoll_ctl( " << m_epfd << ", " << op << ", " << fd << ", "
            << (EPOLL_EVENTS)epevent.events << " ): " << rt << " ( " << errno
            << "-" << strerror(errno)
            << " ) fd_ctx = " << (EPOLL_EVENTS)fd_ctx->events;
        return false;
    }

    if (fd_ctx->events & WRITE) {
        fd_ctx->triggerEvent(WRITE);
        m_penddingEventCount--;
    }

    if (fd_ctx->events & READ) {
        fd_ctx->triggerEvent(READ);
        m_penddingEventCount--;
    }

    LYON_ASSERT(!fd_ctx->events);

    return true;
}

IOManager *IOManager::GetCurrentIOManager() {
    return dynamic_cast<IOManager *>(Scheduler::GetCurrentScheduler());
}

void IOManager::idle() {
    LYON_LOG_INFO(g_logger) << "IOManager idle";
    const int MAX_EVENTSIZE = 1024;
    epoll_event *epevents = new epoll_event[MAX_EVENTSIZE];
    //创建shared_ptr的第二个参数是释放时调用的函数。默认的是delete ptr。
    //通过这种方式可以达到释放数组的操作。使用智能指针而不是在最后delete是为了预防函数运行过程中出错，从而没有释放空间
    std::shared_ptr<epoll_event> sepevents(
        epevents, [](epoll_event *ptr) { delete[] ptr; });

    while (true) {
        uint64_t next_timeout = 0;
        if (stopping(next_timeout)) {
            LYON_LOG_INFO(g_logger) << "IOManager idle stop";
            break;
        }
        int rt = 0;
        //当出现中断错误时，重试
        do {
            //设置超时时间为下次定时时间的到时时间
            static const uint64_t MAX_TIMEOUT = 4000;
            if (next_timeout == ~0ull) {
                next_timeout = MAX_TIMEOUT;
            } else {
                next_timeout =
                    next_timeout < MAX_TIMEOUT ? next_timeout : MAX_TIMEOUT;
            }
            errno = 0;
            rt = epoll_wait(m_epfd, epevents, MAX_EVENTSIZE, next_timeout);
        } while (rt == -1 && errno == EINTR);

        //查询到时的定时器
        std::vector<std::function<void()>> cbs;
        listExpiredCbs(cbs);
        if (!cbs.empty()) {
            addJobs(cbs.begin(), cbs.end());
            cbs.clear();
        }

        for (int i = 0; i < rt; i++) {
            epoll_event &epevent = epevents[i];
            if (epevents->data.fd == m_tickleFds[0]) {
                uint8_t dummy[256];
                while (read(m_tickleFds[0], dummy, sizeof(dummy)) > 0)
                    ;
                continue;
            }
            FdContext *fd_ctx = static_cast<FdContext *>(epevent.data.ptr);
            FdContext::MutexType::Lock lock(fd_ctx->mutex);

            if (epevent.events & (EPOLLERR | EPOLLHUP)) {
                epevent.events |= (EPOLLIN | EPOLLOUT) & fd_ctx->events;
            }
            int real_events = NONE;
            if (epevent.events & EPOLLIN) {
                real_events |= READ;
            }

            if (epevent.events & EPOLLOUT) {
                real_events |= WRITE;
            }

            if ((epevents->events & real_events) == NONE) {
                continue;
            }
            int left_events = (fd_ctx->events & ~real_events);
            int op = left_events ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
            epevent.events = EPOLLET | left_events;
            int rt2 = epoll_ctl(m_epfd, op, fd_ctx->fd, &epevent);
            if (rt2) {
                LYON_LOG_ERROR(g_logger)
                    << "IOManager: "
                    << "epoll_ctl( " << m_epfd << ", " << op << ", "
                    << fd_ctx->fd << ", " << (EPOLL_EVENTS)epevent.events
                    << " ): " << rt << " ( " << errno << "-" << strerror(errno)
                    << " ) fd_ctx = " << (EPOLL_EVENTS)fd_ctx->events;
                continue;
            }

            if (real_events & READ) {
                fd_ctx->triggerEvent(READ);
                m_penddingEventCount--;
            }

            if (real_events & WRITE) {
                fd_ctx->triggerEvent(WRITE);
                m_penddingEventCount--;
            }
        }
        Fiber::HoldToScheduler();
    }
}

void IOManager::tickle() {
    if (!hasIdleThreads()) {
        return;
    }
    int rt = write(m_tickleFds[1], "T", 1);
    LYON_ASSERT(rt == 1)
}

bool IOManager::stopping() {
    uint64_t next_timeout = 0;
    return stopping(next_timeout);
}

bool IOManager::stopping(uint64_t &next_timeout) {
    next_timeout = getNextTimer();
    return next_timeout == ~0ull && Scheduler::stopping() &&
           (m_penddingEventCount == 0);
}

void IOManager::onTimerInsertAtFront() { tickle(); }

IOManager::FdContext::EventContext &
IOManager::FdContext::getEventContext(Event e) {
    switch (e) {
    case READ:
        return read;
    case WRITE:
        return write;
    default:
        LYON_ASSERT2(false, "getEventContext: invalid event type");
    }
    throw std::invalid_argument("getEventContext: invalid event type");
}
void IOManager::FdContext::resetEventContext(EventContext &ctx) {
    ctx.scheduler = nullptr;
    ctx.fiber.reset();
    ctx.cb = nullptr;
}
void IOManager::FdContext::triggerEvent(Event e) {
    LYON_ASSERT(events & e);
    EventContext &ctx = getEventContext(e);
    events = static_cast<Event>(events & (~e));

    if (ctx.cb) {
        GetCurrentScheduler()->addJob(&ctx.cb);
    } else {
        GetCurrentScheduler()->addJob(&ctx.fiber);
    }
    resetEventContext(ctx);
}

} // namespace lyon
