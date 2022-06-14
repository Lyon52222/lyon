#ifndef __LYON_RPC_SERVER_H__
#define __LYON_RPC_SERVER_H__

#include "lyon/tcp_server.h"
#include "rpc_method.h"
#include "rpc_protocol.h"
#include "rpc_session.h"
#include <lyon/address.h>
namespace lyon::rpc {

class RpcServer : public TcpServer {
public:
    typedef std::shared_ptr<RpcServer> ptr;

    using TcpServer::TcpServer;

    RpcServer(IOManager *worker = IOManager::GetCurrentIOManager(),
              IOManager *ioworker = IOManager::GetCurrentIOManager(),
              IOManager *acceptWorker = IOManager::GetCurrentIOManager());

    virtual ~RpcServer();

    virtual bool start() override;

    bool bindRegister(Address::ptr addr);

    bool bindRegister(const std::string &host);

    virtual void handleClient(Socket::ptr sock) override;

    void registMethod(RpcMethod::ptr method);

    bool registMethodToRegister(RpcMethod::ptr method);

    virtual RpcProtocol::ptr handleMethodRequest(RpcProtocol::ptr protocol);

    template <typename F> void registMethod(const std::string &name, F func) {
        auto func_ptr(new RpcMethod(name, func));
        m_methods.emplace(name, func_ptr);
    }

    RpcMethod::ptr getMethod(const std::string &name);

private:
    std::multimap<std::string, RpcMethod::ptr> m_methods;
    RpcSession::ptr m_registerSession;
};
} // namespace lyon::rpc

#endif // !__LYON_RPC_SERVER_H__
