#ifndef __LYON_RPC_SERVER_H__
#define __LYON_RPC_SERVER_H__

#include <lyon/tcp_server.h>
namespace lyon::rpc {

class RPCServer : public TcpServer {
public:
    typedef std::shared_ptr<RPCServer> ptr;

    using TcpServer::TcpServer;

    virtual void handleClient(Socket::ptr sock) override;

private:
};
} // namespace lyon::rpc

#endif // !__LYON_RPC_SERVER_H__
