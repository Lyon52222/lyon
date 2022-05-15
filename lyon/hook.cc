#include "hook.h"
#include "iomanager.h"
#include "log.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <cstdint>
#include <dlfcn.h>
#include <fdmanager.h>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>

namespace lyon {

static lyon::Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static thread_local bool s_hook_enable = false;
static uint64_t s_connect_timeout = -1;

#define HOOK_FUN(XX)                                                           \
    XX(sleep)                                                                  \
    XX(usleep)                                                                 \
    XX(socket)                                                                 \
    XX(connect)                                                                \
    XX(accept)                                                                 \
    XX(read)                                                                   \
    XX(readv)                                                                  \
    XX(recv)                                                                   \
    XX(recvfrom)                                                               \
    XX(recvmsg)                                                                \
    XX(write)                                                                  \
    XX(writev)                                                                 \
    XX(send)                                                                   \
    XX(sendto)                                                                 \
    XX(sendmsg)                                                                \
    XX(close)                                                                  \
    XX(fcntl)                                                                  \
    XX(ioctl)                                                                  \
    XX(getsockopt)                                                             \
    XX(setsockopt)

void hook_init() {
//将name_f 指向原函数
#define XX(name) name##_f = (name##_fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX)
#undef XX
}

struct _HookIniter {
    _HookIniter() { hook_init(); }
};

static _HookIniter s_hook_initer;

bool is_hook_enable() { return s_hook_enable; }

void set_hook_enable(bool flag) { s_hook_enable = flag; }

struct timer_cond {
    int cancelled = 0;
};

template <typename OrginFun, typename... Args>
static ssize_t do_io(int fd, OrginFun fun, const char *hook_fun_name,
                     uint32_t event, int timeout_so, Args &&...args) {
    if (!lyon::s_hook_enable) {
        return fun(fd, std::forward<Args>(args)...);
    }
    FdCtx::ptr ctx = FdMgr::getInstance()->get(fd);
    if (!ctx) {
        return fun(fd, std::forward<Args>(args)...);
    }
    if (ctx->isClose()) {
        errno = EBADF;
        return -1;
    }
    if (!ctx->isSockt() || ctx->getUsrNonblock()) {
        return fun(fd, std::forward<Args>(args)...);
    }
    uint64_t to = ctx->getTimeout(timeout_so);
    std::shared_ptr<timer_cond> tcond(new timer_cond);

retry:
    ssize_t n = fun(fd, std::forward<Args>(args)...);
    //中断重试
    while (n == -1 && errno == EINTR) {
        n = fun(fd, std::forward<Args>(args)...);
    }
    //无数据阻塞
    if (n == -1 && errno == EAGAIN) {
        IOManager *iom = IOManager::GetCurrentIOManager();
        Fiber::ptr fiber = Fiber::GetCurrentFiber();
        Timer::ptr timer = nullptr;
        std::weak_ptr<timer_cond> wtcond(tcond);
        if (to != static_cast<uint64_t>(-1)) {
            timer = iom->addConditionTimer(
                to,
                [&iom, &fiber, &wtcond, &fd, &event]() {
                    auto t = wtcond.lock();
                    if (!t || t->cancelled) {
                        return;
                    }
                    //设置超时错误
                    t->cancelled = ETIMEDOUT;
                    iom->triggerEvent(
                        fd, static_cast<lyon::IOManager::Event>(event));
                },
                wtcond);
        }
        int rt = iom->addEvent(fd, static_cast<lyon::IOManager::Event>(event));
        if (rt) {
            LYON_LOG_ERROR(g_logger) << hook_fun_name << "add event : (" << fd
                                     << ", " << event << " ) fail";
            if (timer) {
                timer->cancel();
                return -1;
            }
        } else {
            Fiber::HoldToScheduler();
            //如果在定时器触发前返回，取消定时器
            if (timer)
                timer->cancel();
            if (tcond->cancelled) {
                //如果io超时了
                errno = tcond->cancelled;
                return -1;
            }
            //时间到了，再次去查询原函数，如果这次准备好了则会直接返回
            goto retry;
        }
    }
    return n;
}

extern "C" {

#define XX(name) name##_fun name##_f = nullptr;
HOOK_FUN(XX)
#undef XX

unsigned int sleep(unsigned int seconds) {
    if (!lyon::s_hook_enable) {
        return sleep_f(seconds);
    }
    lyon::IOManager *iom = lyon::IOManager::GetCurrentIOManager();
    lyon::Fiber::ptr fiber = lyon::Fiber::GetCurrentFiber();

    // 设置一个定时器，然后让出执行
    // iom->addTimer(seconds * 1000,
    //               std::bind((void(Scheduler::*)(Fiber::ptr, bool s_thread,
    //                                             int
    //                                             thread))IOManager::addJob,
    //                         iom, fiber, false, 0));
    iom->addTimer(seconds * 1000, [&iom, &fiber]() { iom->addJob(fiber); });

    lyon::Fiber::HoldToScheduler();
    return 0;
}

int usleep(__useconds_t useconds) {
    if (!lyon::s_hook_enable) {
        return sleep_f(useconds);
    }
    lyon::IOManager *iom = lyon::IOManager::GetCurrentIOManager();
    lyon::Fiber::ptr fiber = lyon::Fiber::GetCurrentFiber();
    iom->addTimer(useconds / 1000, [&iom, &fiber]() { iom->addJob(fiber); });
    lyon::Fiber::HoldToScheduler();
    return 0;
}

int socket(int domain, int type, int protocol) {
    if (!lyon::s_hook_enable) {
        return socket_f(domain, type, protocol);
    }
    int sfd = socket(domain, type, protocol);
    if (sfd != -1) {
        lyon::FdMgr::getInstance()->get(sfd, true);
    }
    return sfd;
}

int connect_with_timeout(int socket, const struct sockaddr *address,
                         socklen_t address_len, uint64_t timeout_ms) {
    if (!lyon::s_hook_enable) {
        return connect_f(socket, address, address_len);
    }
    //获取相应的fd描述
    lyon::FdCtx::ptr ctx = lyon::FdMgr::getInstance()->get(socket);
    //如果为空（未创建）或已经关闭
    if (!ctx || ctx->isClose()) {
        errno = EBADF;
        return -1;
    }
    //如果不是socket连接或者用户设置未非阻塞
    if (!ctx->isSockt() || ctx->getUsrNonblock()) {
        return connect_f(socket, address, address_len);
    }

    int n = connect_f(socket, address, address_len);
    if (n == 0) {
        //连接成功
        return 0;
    } else if (n != -1 || errno != EINPROGRESS) {
        return -1;
    }
    //因为socket是非阻塞的所以connect回直接返回-1并且设置errno为EINPROGRESS
    IOManager *iom = IOManager::GetCurrentIOManager();
    std::shared_ptr<timer_cond> tcond(new timer_cond);
    std::weak_ptr<timer_cond> wtcond(tcond);
    Timer::ptr timer = nullptr;
    //如果设置了超时事件，则添加一个条件定时器，触发后面添加的event
    if (timeout_ms != static_cast<uint64_t>(-1)) {
        timer = iom->addConditionTimer(
            timeout_ms,
            [&iom, &socket, &wtcond]() {
                auto t = wtcond.lock();
                if (!t || t->cancelled) {
                    return;
                }
                //设置超时错误
                t->cancelled = ETIMEDOUT;
                //手动触发event事件返回
                iom->triggerEvent(socket, IOManager::WRITE);
            },
            wtcond);
    }
    int rt = iom->addEvent(socket, IOManager::WRITE);
    if (!rt) {
        Fiber::HoldToScheduler();
        // 如果定时器还没到时就被触发了
        if (timer) {
            timer->cancel();
        }
        if (tcond->cancelled) {
            errno = tcond->cancelled;
            return -1;
        }
    } else {
        if (timer) {
            timer->cancel();
            LYON_LOG_ERROR(g_logger) << "connect_with_timeout - add event : ("
                                     << socket << "WRITE fail";
        }
    }
    //最后验证连接是否成功
    int error = 0;
    socklen_t len = sizeof(int);
    if (-1 == getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, &len)) {
        return -1;
    }
    if (error) {
        errno = error;
        return -1;
    } else {
        return 0;
    }
    //如果中途发生了错误导致函数直接推出的话，也会导致tcond失效。timer不会触发
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len) {
    return connect_with_timeout(socket, address, address_len,
                                lyon::s_connect_timeout);
}

int accept(int socket, struct sockaddr *address, socklen_t *address_len) {
    int fd = do_io(socket, accept_f, "accept", IOManager::READ, SO_RCVTIMEO,
                   address, address_len);
    if (fd >= 0) {
        FdMgr::getInstance()->get(fd, true);
    }
    return fd;
}

ssize_t read(int fildes, void *buf, size_t nbyte) {
    return do_io(fildes, read_f, "read", IOManager::READ, SO_RCVTIMEO, buf,
                 nbyte);
}

ssize_t readv(int fildes, const struct iovec *iov, int iovcnt) {
    return do_io(fildes, readv_f, "readv", IOManager::READ, SO_RCVTIMEO, iov,
                 iovcnt);
}

ssize_t recv(int socket, void *buffer, size_t length, int flags) {
    return do_io(socket, recv_f, "recv", IOManager::READ, SO_RCVTIMEO, buffer,
                 length, flags);
}

ssize_t recvfrom(int socket, void *buffer, size_t length, int flags,
                 struct sockaddr *address, socklen_t *address_len) {
    return do_io(socket, recvfrom_f, "recvfrom", IOManager::READ, SO_RCVTIMEO,
                 buffer, length, flags, address, address_len);
}

ssize_t recvmsg(int socket, struct msghdr *message, int flags) {
    return do_io(socket, recvmsg_f, "recvmsg", IOManager::READ, SO_RCVTIMEO,
                 message, flags);
}

ssize_t write(int fildes, const void *buf, size_t nbyte) {
    return do_io(fildes, write_f, "write", IOManager::WRITE, SO_SNDTIMEO, buf,
                 nbyte);
}

ssize_t writev(int fildes, const struct iovec *iov, int iovcnt) {
    return do_io(fildes, writev_f, "writev", IOManager::WRITE, SO_SNDTIMEO, iov,
                 iovcnt);
}

ssize_t send(int socket, const void *buffer, size_t length, int flags) {
    return do_io(socket, send_f, "send", IOManager::WRITE, SO_SNDTIMEO, buffer,
                 length, flags);
}

ssize_t sendto(int socket, const void *message, size_t length, int flags,
               const struct sockaddr *dest_addr, socklen_t dest_len) {
    return do_io(socket, sendto_f, "sendto", IOManager::WRITE, SO_SNDTIMEO,
                 message, length, flags, dest_addr, dest_len);
}

ssize_t sendmsg(int socket, const struct msghdr *message, int flags) {
    return do_io(socket, sendmsg_f, "sendmsg", IOManager::WRITE, SO_SNDTIMEO,
                 message, flags);
}

int close(int fildes) { return close_f(fildes); }

int fcntl(int fildes, int cmd, ...) { return fcntl_f(fildes, cmd); }

int ioctl(int fildes, int request, ... /* arg */) {
    return ioctl_f(fildes, request);
}

int getsockopt(int socket, int level, int option_name, void *option_value,
               socklen_t *option_len) {
    return getsockopt_f(socket, level, option_name, option_value, option_len);
}

int setsockopt(int socket, int level, int option_name, const void *option_value,
               socklen_t option_len) {
    return setsockopt(socket, level, option_name, option_value, option_len);
}
}
} // namespace lyon
