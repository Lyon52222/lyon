#ifndef __LYON_RPC_CONNECTION_POLL_H__
#define __LYON_RPC_CONNECTION_POLL_H__

#include "lyon/mutex.h"
#include "lyon/serialize/serializer.h"
#include "rpc_connection.h"
#include "rpc_method.h"
#include "rpc_result.h"
#include "rpc_session.h"
#include <cstdint>
#include <memory>

namespace lyon::rpc {

class RpcConnectionPool
    : public std::enable_shared_from_this<RpcConnectionPool> {
public:
    typedef std::shared_ptr<RpcConnectionPool> ptr;
    typedef Mutex MutexType;

    RpcConnectionPool(uint32_t maxSize, uint32_t maxLiveTime,
                      uint32_t maxRequest);

    static RpcConnectionPool::ptr Create(uint32_t maxSize, uint32_t maxLiveTime,
                                         uint32_t maxRequest);

    bool bindRegister(Address::ptr addr);

    bool bindRegister(const std::string &host);

    RpcConnection::ptr getConnection(const RpcMethodMeta &method);

    std::vector<std::string> discover(const RpcMethodMeta &method);

    void reportServerError(std::string server);

    static void ReleasePtr(RpcConnection *ptr, RpcConnectionPool *poll,
                           RpcMethodMeta method);

    template <typename T, typename... Args>
    std::future<RpcResult<T>> future_call(const std::string &name,
                                          const Args &...args) {
        std::function<RpcResult<T>()> call_job = [this, name,
                                                  args...]() -> RpcResult<T> {
            return call<T>(name, args...);
        };

        // promise用于产生future
        auto promise = std::make_shared<std::promise<RpcResult<T>>>();

        RpcConnectionPool::ptr this_ptr = shared_from_this();
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
        RpcConnectionPool::ptr this_ptr = shared_from_this();

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
        RpcMethodMeta method(name, rt_type, args_type);
        auto conn = getConnection(method);
        if (!conn) {
            return RpcResult<T>(RpcResultState::GET_CONNECTION_ERROR,
                                "Get connection error");
        }
        return conn->call<T>(call_ser);
    }

private:
    uint32_t m_maxSize;
    uint32_t m_maxLiveTime;
    uint32_t m_maxRequest;
    std::atomic<uint32_t> m_total = {0};
    RpcSession::ptr m_registerSession;
    //每个方法所对应的连接池
    std::map<RpcMethodMeta, std::list<RpcConnection *>> m_connections;
    //每个方法所对应的可用服务器
    std::map<RpcMethodMeta, std::list<std::string>> m_servers;
    MutexType m_mutex;
};

} // namespace lyon::rpc
#endif
