#ifndef __LYON_RPC_CONNECTION_POLL_H__
#define __LYON_RPC_CONNECTION_POLL_H__

#include "lyon/mutex.h"
#include <cstdint>
#include <memory>
#include <rpc_client.h>
#include <rpc_method.h>

namespace lyon::rpc {

class RpcConnectionPoll {
public:
    typedef std::shared_ptr<RpcConnectionPoll> ptr;
    typedef Mutex MutexType;

    RpcConnectionPoll(uint32_t maxSize, uint32_t maxLiveTime,
                      uint32_t maxRequest);

    std::vector<std::string> discover(const RpcMethodMeta &method);

private:
    uint32_t m_maxSize;
    uint32_t m_maxLiveTime;
    uint32_t m_maxRequest;

    std::map<std::string, std::list<RpcClient *>> m_connections;

    MutexType m_mutex;
};

} // namespace lyon::rpc
#endif
