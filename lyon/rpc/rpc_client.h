#ifndef __LYON_RPC_CLIENT_H__
#define __LYON_RPC_CLIENT_H__
#include "lyon/address.h"
#include "lyon/serialize/serializer.h"
#include "lyon/socket.h"
#include "rpc_protocol.h"
#include "rpc_result.h"
#include "rpc_session.h"
#include <cstdint>
#include <lyon/log.h>
#include <memory>
namespace lyon::rpc {

class RpcClient {
public:
    typedef std::shared_ptr<RpcClient> ptr;

    RpcClient(uint64_t timeoutMs) : m_timeoutMs(timeoutMs) {}

    [[nodiscard]] bool connect(Address::ptr addr);

    [[nodiscard]] bool connect(const std::string &host);

    template <typename T, typename... Args>
    RpcResult<T> call(const std::string &name, Args... args) {
        auto args_tuple = std::make_tuple(args...);
        Serializer call_ser;
        call_ser << name << args_tuple;
        return call<T>(call_ser);
    }

    template <typename T> RpcResult<T> call(Serializer &ser) {
        if (!m_session || !m_session->isConnected()) {
            return RpcResult<T>(RpcResultState::NOT_CONNECT, "Socket Closed");
        }

        //创建函数调用请求
        RpcProtocol::ptr request = RpcProtocol::CreateMethodRequest();
        //将函数的参数放入请求体中
        request->setContent(ser.toString());

        LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
            << "Client send request" << request->toString();

        //发送函数调用请求
        int rt = m_session->sendRpcProtocol(request);

        if (rt == 0) {
            return RpcResult<T>(RpcResultState::SEND_CLOSE_BY_PEER,
                                "Send close by peer");
        } else if (rt < 0) {
            return RpcResult<T>(RpcResultState::SEND_SOCKET_ERROR,
                                "Send socket error");
        }

        //接收函数调用请求
        RpcProtocol::ptr response = m_session->recvRpcProtocol();

        LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
            << "Client recv response" << response->toString();

        //将收到的响应体解析为序列化数据
        Serializer result_ser(response->getContent(), response->isCompress());

        RpcResult<T> result;
        result_ser >> result;

        return result;
    }

private:
    RpcSession::ptr m_session = nullptr;
    uint64_t m_timeoutMs = -1;
};

} // namespace lyon::rpc
#endif
