#ifndef __LYON_RPC_REGISTER_H__
#define __LYON_RPC_REGISTER_H__

#include <lyon/tcp_server.h>
#include <memory>
namespace lyon::rpc {

class RPCGegister : public TcpServer {
public:
    typedef std::shared_ptr<RPCGegister> ptr;
    using TcpServer::TcpServer;

    virtual void handleClient(Socket::ptr sock) override;

private:
};

} // namespace lyon::rpc

#endif
