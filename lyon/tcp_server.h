#ifndef __LYON_TCP_SERVER_H__
#define __LYON_TCP_SERVER_H__
#include "iomanager.h"
#include "socket.h"
#include <boost/core/noncopyable.hpp>
#include <cstdint>
#include <functional>
#include <memory>

namespace lyon {
class TcpServer : public std::enable_shared_from_this<TcpServer>,
                  public boost::noncopyable {
public:
    typedef std::shared_ptr<TcpServer> ptr;

    TcpServer(IOManager *worker = IOManager::GetCurrentIOManager(),
              IOManager *ioworker = IOManager::GetCurrentIOManager(),
              IOManager *acceptWorker = IOManager::GetCurrentIOManager());

    ~TcpServer();

    bool bindAndListen(const Address::ptr addr, bool ssl = 0);

    /**
     * @brief 尝试对addrs中的地址创建socket并绑定并开始监听
     *
     * @param addrs 需要绑定的地址
     * @param failed_addrs 绑定失败的地址
     * @return 是否绑定成功
     */
    bool bindAndListen(const std::vector<Address::ptr> &addrs,
                       std::vector<Address::ptr> &failed_addrs, bool ssl = 0);

    bool start();
    void stop();

    virtual void handleClient(Socket::ptr sock);

private:
    void startAccept(const Socket::ptr sock);

private:
    std::vector<Socket::ptr> m_sockets;
    IOManager *m_worker;
    IOManager *m_ioworker;
    IOManager *m_acceptWorker;
    uint64_t m_recvTimeout;
    std::string m_name;
    bool m_isStop;
};

} // namespace lyon

#endif
