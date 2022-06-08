#ifndef __LYON_RPC_SERVER_H__
#define __LYON_RPC_SERVER_H__

#include "lyon/tcp_server.h"
#include "rpc_method.h"
#include "rpc_protocol.h"
namespace lyon::rpc {

class RpcServer : public TcpServer {
public:
    typedef std::shared_ptr<RpcServer> ptr;

    using TcpServer::TcpServer;

    RpcServer(IOManager *worker = IOManager::GetCurrentIOManager(),
              IOManager *ioworker = IOManager::GetCurrentIOManager(),
              IOManager *acceptWorker = IOManager::GetCurrentIOManager());

    virtual void handleClient(Socket::ptr sock) override;

    void registMethod(RPCMethod::ptr method);

    virtual RpcProtocol::ptr handleMethodRequest(RpcProtocol::ptr protocol);

    template <typename F> void registMethod(const std::string &name, F func) {
        auto func_ptr(new RPCMethod(name, func));
        m_methods.emplace(name, func_ptr);
    }

    RPCMethod::ptr getMethod(const std::string &name);

private:
    std::map<std::string, RPCMethod::ptr> m_methods;
};
} // namespace lyon::rpc

#endif // !__LYON_RPC_SERVER_H__
