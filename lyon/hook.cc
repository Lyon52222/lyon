#include "hook.h"
#include "iomanager.h"
#include "log.h"
#include <asm-generic/errno-base.h>
#include <cstdint>
#include <dlfcn.h>
#include <fdmanager.h>
#include <memory>
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
                     uint32_t event, int timeout_so, ssize_t buflen,
                     Args &&...args) {
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

    ssize_t n = fun(fd, std::forward<Args>(args)...);
    //中断重试
    while (n == -1 && errno == EINTR) {
        n = fun(fd, std::forward<Args>(args)...);
    }
    //无数据阻塞
    if (n == -1 && errno == EAGAIN) {
        IOManager *iom = IOManager::GetCurrentIOManager();
        Fiber::ptr fiber = Fiber::GetCurrentFiber();
        std::weak_ptr<timer_cond> wtcond(tcond);
        // TODO: last place
    }
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

    //通过设置一个定时器事件来唤醒
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
                         socklen_t address_len, uint64_t timeout) {
    if (!lyon::s_hook_enable) {
        return connect_f(socket, address, address_len);
    }
    lyon::FdCtx::ptr ctx = lyon::FdMgr::getInstance()->get(socket);
    if (!ctx || ctx->isClose()) {
        errno = EBADF;
        return -1;
    }
    if (!ctx->isSockt() || ctx->getUsrNonblock()) {
        return connect_f(socket, address, address_len);
    }

    // TODO: last place
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len) {
    return connect_with_timeout(socket, address, address_len,
                                lyon::s_connect_timeout);
}

int accept(int socket, struct sockaddr *address, socklen_t *address_len) {}

ssize_t read(int fildes, void *buf, size_t nbyte) {}

ssize_t readv(int fildes, const struct iovec *iov, int iovcnt) {}

ssize_t recv(int socket, void *buffer, size_t length, int flags) {}

ssize_t recvfrom(int socket, void *buffer, size_t length, int flags,
                 struct sockaddr *address, socklen_t *address_len) {}

ssize_t recvmsg(int socket, struct msghdr *message, int flags) {}

ssize_t write(int fildes, const void *buf, size_t nbyte) {}

ssize_t writev(int fildes, const struct iovec *iov, int iovcnt) {}

ssize_t send(int socket, const void *buffer, size_t length, int flags) {}

ssize_t sendto(int socket, const void *message, size_t length, int flags,
               const struct sockaddr *dest_addr, socklen_t dest_len) {}

ssize_t sendmsg(int socket, const struct msghdr *message, int flags) {}

int close(int fildes) {}

int fcntl(int fildes, int cmd, ...) {}

int ioctl(int fildes, int request, ... /* arg */) {}

int getsockopt(int socket, int level, int option_name, void *option_value,
               socklen_t *option_len) {}
}

int setsockopt(int socket, int level, int option_name, const void *option_value,
               socklen_t option_len) {}
} // namespace lyon
