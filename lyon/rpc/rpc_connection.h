#ifndef __LYON_RPC_CLIENT_H__
#define __LYON_RPC_CLIENT_H__
#include "lyon/address.h"
#include "lyon/serialize/serializer.h"
#include "lyon/socket.h"
#include "rpc_protocol.h"
#include "rpc_result.h"
#include "rpc_session.h"
#include <cstdint>
#include <future>
#include <lyon/iomanager.h>
#include <lyon/log.h>
#include <memory>
namespace lyon::rpc {

class RpcConnection : public std::enable_shared_from_this<RpcConnection> {
public:
    typedef std::shared_ptr<RpcConnection> ptr;

    RpcConnection(uint64_t timeoutMs = -1);

    ~RpcConnection();

    [[nodiscard]] bool connect(Address::ptr addr);

    [[nodiscard]] bool connect(const std::string &host);

    [[nodiscard]] bool isConnected();

    [[nodiscard]] uint64_t getCreateTime();

    uint32_t incRequest() { return ++m_request; }
    uint32_t decRequest() { return --m_request; }
    uint32_t getRequest() const { return m_request; }

    template <typename T, typename... Args>
    std::future<RpcResult<T>> future_call(const std::string &name,
                                          const Args &...args) {
        std::function<RpcResult<T>()> call_job = [this, name,
                                                  args...]() -> RpcResult<T> {
            return call<T>(name, args...);
        };

        // promise用于产生future
        auto promise = std::make_shared<std::promise<RpcResult<T>>>();

        RpcConnection::ptr this_ptr = shared_from_this();
        IOManager::GetCurrentIOManager()->addJob(
            [this_ptr, promise, call_job]() mutable {
                //调用成功后设置value,是的future有效
                promise->set_value(call_job());
                this_ptr = nullptr;
            });

        return promise->get_future();
    }

    template <typename T, typename... Args>
    void async_call(std::function<void(RpcResult<T>)> call_back,
                    const std::string &name, const Args &...args) {
        //这里传递this可能出现的问题就是，当调度到call_job的时候可能this已经被析构了。

        std::function<RpcResult<T>()> call_job = [this, name,
                                                  args...]() -> RpcResult<T> {
            return call<T>(name, args...);
        };

        //所以这里使用shared_ptr来保持，延缓当前client的析构
        RpcConnection::ptr this_ptr = shared_from_this();

        // lambda表达式值捕获是在lambda表达式定义的时候进行捕获，而引用捕获则实在调用时进行的捕获。//因此这里如果采用引用捕获的话也会有函数调用时变量失效的问题
        IOManager::GetCurrentIOManager()->addJob(
            [this_ptr, call_back, call_job]() mutable {
                call_back(call_job());
                this_ptr = nullptr;
            });
    }

    template <typename T, typename... Args>
    RpcResult<T> call(const std::string &name, const Args &...args) {
        auto args_tuple = std::make_tuple(args...);
        Serializer call_ser;
        std::string rt_type = typeid(T).name();
        std::string args_type = typeid(args_tuple).name();
        call_ser << name << rt_type << args_type << args_tuple;
        return call<T>(call_ser);
    }

    template <typename T> RpcResult<T> call(Serializer &ser) {
        if (!m_session || !m_session->isConnected()) {
            return RpcResult<T>(RpcResultState::NOT_CONNECT, "Socket Closed");
        }

        //创建函数调用请求
        RpcProtocol::ptr request = RpcProtocol::CreateCallMethodRequest();
        //将函数的参数放入请求体中
        request->setContent(ser.toString());

        Socket::ptr sock = m_session->getSocket();

        sock->setSendTimeout(m_timeoutMs);

        //发送函数调用请求
        int rt = m_session->sendRpcProtocol(request);

        if (rt == 0) {
            return RpcResult<T>(RpcResultState::SEND_CLOSE_BY_PEER,
                                "Send close by peer");
        } else if (rt < 0) {
            return RpcResult<T>(RpcResultState::SEND_SOCKET_ERROR,
                                "Send socket error");
        }

        LYON_LOG_DEBUG(LYON_LOG_GET_LOGGER("system"))
            << "Client send request" << request->toString();

        // TODO:这里可能存在的问题有：1.服务端处理需要时间，2.客户端接收到的应答顺序可能和请求的顺序不一样

        //接收函数调用请求
        RpcProtocol::ptr response = m_session->recvRpcProtocol();

        if (!response) {
            return RpcResult<T>(RpcResultState::RECV_BAD_RESPONSE,
                                "Recv bad response");
        }

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
    uint32_t m_request = 0;
    uint64_t m_timeoutMs = -1;
    uint64_t m_createTime;
};

} // namespace lyon::rpc
#endif
