#ifndef __LYON_RPC_REGISTER_H__
#define __LYON_RPC_REGISTER_H__

#include "lyon/tcp_server.h"
#include "rpc_method.h"
#include "rpc_protocol.h"
#include "rpc_session.h"
#include <lyon/address.h>
#include <memory>
namespace lyon::rpc {

class RpcRegister : public TcpServer {
public:
    typedef std::shared_ptr<RpcRegister> ptr;
    using TcpServer::TcpServer;

    bool aliveTest(const std::string &server);

    virtual void handleClient(Socket::ptr sock) override;

    /**
     * @brief 处理方法注册
     *
     * @param session 对应的session
     */
    RpcProtocol::ptr handleRegistMethod(RpcProtocol::ptr request,
                                        Address::ptr server_addr);

    RpcProtocol::ptr handleDiscoverMethod(RpcProtocol::ptr request);

    void handleServerError(RpcProtocol::ptr request);

private:
    //注册的方法和其对应服务器
    std::map<RpcMethodMeta, std::list<std::string>> m_registedMethod;
    //服务器和其提供的方法
    std::map<std::string, std::list<std::string>::const_iterator>
        m_serverMethod;
};

} // namespace lyon::rpc

#endif
