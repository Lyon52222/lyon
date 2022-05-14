#ifndef __LYON_FD_MANAGER_H__
#define __LYON_FD_MANAGER_H__
#include "iomanager.h"
#include "mutex.h"
#include "singleton.h"
#include "thread.h"
#include <cstdint>
#include <memory>
#include <vector>
namespace lyon {

class FdCtx : public std::enable_shared_from_this<FdCtx> {
public:
    typedef std::shared_ptr<FdCtx> ptr;
    FdCtx(int fd);
    bool init();
    bool isInit() const { return m_isInit; }
    bool isSockt() const { return m_isSockt; }
    bool isClose() const { return m_isClose; }
    void setSysNonblock(bool val) { m_sysNonblock = val; }
    bool getSysNonvlock() const { return m_sysNonblock; }
    void setUsrNonblock(bool val) { m_usrNonblock = val; }
    bool getUsrNonblock() const { return m_usrNonblock; }

    void setTimeout(int type, int64_t val);
    int64_t getTimeout(int type);

    ~FdCtx();

private:
    bool m_isInit : 1;
    bool m_isSockt : 1;
    bool m_isClose : 1;
    bool m_sysNonblock : 1;
    bool m_usrNonblock : 1;
    int m_fd;
    uint64_t m_recvTimeout;
    uint64_t m_sendTimeout;
};

class FdManager {
public:
    typedef RWMutex RWMutexType;
    FdManager();
    FdCtx::ptr get(int fd, bool auto_create = false);
    void del(int fd);

private:
    RWMutexType m_mutex;
    std::vector<FdCtx::ptr> m_fds;
};

typedef Singleton<FdManager> FdMgr;

} // namespace lyon
#endif
