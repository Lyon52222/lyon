#include "iomanager.h"
#include "log.h"
#include "macro.h"
#include <cstddef>
#include <exception>
#include <fcntl.h>
#include <scheduler.h>
#include <stdexcept>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
namespace lyon {

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
    rt = fcntl(m_tickleFds[0], F_SETFL | O_NONBLOCK);
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

    if (fd_ctx->events & event) {
        LYON_LOG_WARN(g_logger) << "IOManager: event exist - event = " << event
                                << " fd_ctx.events = " << fd_ctx->events;
        return -1;
    }

    int op = fd_ctx->events ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    epoll_event epevent;
    epevent.events = EPOLLET | fd_ctx->events | event;
    //设置事件对应的附加 数据指针，后续可以重新得到。
    epevent.data.ptr = fd_ctx;

    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "IOManager: "
            << "epoll_ctl( " << m_epfd << ", " << op << ", " << fd << ", "
            << epevent.events << " ): " << rt << " ( " << errno << "-"
            << strerror(errno) << " ) fd_ctx = " << fd_ctx->events;
        return -1;
    }
    m_penddingEventCount++;

    fd_ctx->events = static_cast<Event>(fd_ctx->events | event);
    auto &event_ctx = fd_ctx->getEventContext(event);
    LYON_ASSERT(!event_ctx.cb && !event_ctx.fiber && !event_ctx.scheduler);
    event_ctx.scheduler = GetCurrentScheduler();
    if (cb) {
        event_ctx.cb.swap(cb);
    } else {
        event_ctx.fiber = Fiber::GetCurrentFiber();
        LYON_ASSERT2(event_ctx.fiber->getState() == Fiber::EXEC,
                     "Fiber state error = " << event_ctx.fiber->getState());
    }

    return 0;
}

int IOManager::deleEvent(int fd, Event event) {
    RWMutexType::RDLock rlock(m_mutex);
    FdContext *fd_ctx = nullptr;
    if (fd >= m_fdContexts.size()) {
        return -1;
    } else {
        fd_ctx = m_fdContexts[fd];
        rlock.unlock();
    }
    FdContext::MutexType::Lock lock(fd_ctx->mutex);

    if (!(fd_ctx->events & event)) {
        LYON_LOG_WARN(g_logger)
            << "IOManager: event not exist - event = " << event
            << " fd_ctx.events = " << fd_ctx->events;
        return -1;
    }

    int op = fd_ctx->events & (~event) ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events = EPOLLET | (fd_ctx->events & (~event));
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if (rt) {
        // TODO: last place
    }

    return 0;
}

bool IOManager::cancleEvent(int fd, Event event) { return true; }

IOManager *IOManager::GetCurrentIOManager() {
    return dynamic_cast<IOManager *>(Scheduler::GetCurrentScheduler());
}

void IOManager::idle() {}

void IOManager::tickle() {}

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

} // namespace lyon
