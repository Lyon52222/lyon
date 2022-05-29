#include "fdmanager.h"
#include "hook.h"
#include <asm-generic/socket.h>
#include <cstdint>
#include <fcntl.h>
#include <sys/stat.h>
namespace lyon {

FdCtx::FdCtx(int fd)
    : m_isInit(false), m_isSockt(false), m_isClose(false), m_sysNonblock(false),
      m_usrNonblock(false), m_fd(fd), m_recvTimeout(-1), m_sendTimeout(-1) {
    init();
}

bool FdCtx::init() {
    if (m_isInit) {
        return true;
    }
    struct stat fd_stat;
    if (fstat(m_fd, &fd_stat)) {
        m_isInit = false;
    } else {
        m_isInit = true;
        m_isSockt = S_ISSOCK(fd_stat.st_mode);
    }

    if (m_isSockt) {
        int flags = fcntl_f(m_fd, F_GETFL, 0);
        if (!(flags & O_NONBLOCK))
            fcntl_f(m_fd, F_SETFL, flags | O_NONBLOCK);
        m_sysNonblock = true;
    } else {
        m_sysNonblock = false;
    }
    m_usrNonblock = false;
    m_isClose = false;
    return m_isInit;
}

void FdCtx::setTimeout(int type, int64_t val) {
    if (type == SO_RCVTIMEO) {
        m_recvTimeout = val;
    } else {
        m_sendTimeout = val;
    }
}

int64_t FdCtx::getTimeout(int type) {
    if (type == SO_RCVTIMEO) {
        return m_recvTimeout;
    } else {
        return m_sendTimeout;
    }
}

FdManager::FdManager() {}

FdCtx::ptr FdManager::get(int fd, bool auto_create) {
    if (fd < 0)
        return nullptr;
    RWMutexType::RDLock rlock(m_mutex);
    if (fd >= static_cast<int>(m_fds.size())) {
        if (!auto_create) {
            return nullptr;
        }
    } else {
        if (m_fds[fd] || !auto_create) {
            return m_fds[fd];
        }
    }
    rlock.unlock();
    RWMutexType::WRLock wlock(m_mutex);
    FdCtx::ptr fdctx(new FdCtx(fd));
    if (fd >= static_cast<int>(m_fds.size())) {
        m_fds.resize(fd * 1.5);
    }
    m_fds[fd] = fdctx;

    return fdctx;
}
void FdManager::del(int fd) {
    RWMutexType::WRLock wlock(m_mutex);
    if (fd < 0 || fd >= static_cast<int>(m_fds.size()) || (!m_fds[fd])) {
        return;
    }
    m_fds[fd].reset();
}

} // namespace lyon
