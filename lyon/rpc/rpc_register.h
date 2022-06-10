#ifndef __LYON_RPC_REGISTER_H__
#define __LYON_RPC_REGISTER_H__

#include <lyon/tcp_server.h>
#include <memory>
#include <rpc_session.h>
namespace lyon::rpc {

class RpcRegister : public TcpServer {
public:
    typedef std::shared_ptr<RpcRegister> ptr;
    using TcpServer::TcpServer;

    virtual void handleClient(Socket::ptr sock) override;

    /**
     * @brief 处理方法注册
     *
     * @param session 对应的session
     */
    void handleServerRegist(RpcSession::ptr session);

private:
};

} // namespace lyon::rpc

#endif
