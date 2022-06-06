#ifndef __LYON_RPC_SERVER_H__
#define __LYON_RPC_SERVER_H__

#include "lyon/tcp_server.h"
#include "rpc_method.h"
namespace lyon::rpc {

class RPCServer : public TcpServer {
public:
    typedef std::shared_ptr<RPCServer> ptr;

    using TcpServer::TcpServer;

    virtual void handleClient(Socket::ptr sock) override;

    void registMethod(IRPCMethod::ptr method);

    IRPCMethod::ptr getMethod(const std::string &name);

private:
    std::map<std::string, IRPCMethod::ptr> m_methods;
};
} // namespace lyon::rpc

#endif // !__LYON_RPC_SERVER_H__
