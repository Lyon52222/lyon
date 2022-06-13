#include "rpc_connection_poll.h"
#include <lyon/log.h>
#include <memory>

namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

RpcConnectionPoll::RpcConnectionPoll(uint32_t maxSize, uint32_t maxLiveTime,
                                     uint32_t maxRequest)
    : m_maxSize(maxSize), m_maxLiveTime(maxLiveTime), m_maxRequest(maxRequest) {
}

RpcConnectionPoll::ptr RpcConnectionPoll::Create(uint32_t maxSize,
                                                 uint32_t maxLiveTime,
                                                 uint32_t maxRequest) {
    return std::make_shared<RpcConnectionPoll>(maxSize, maxLiveTime,
                                               maxRequest);
}

bool RpcConnectionPoll::bindRegister(Address::ptr addr) {
    Socket::ptr sock = Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        LYON_LOG_ERROR(g_logger) << "RpcServer::bindRegister fail";
        m_registerSession = nullptr;
        return false;
    }
    m_registerSession = std::make_shared<RpcSession>(sock);
    return true;
}

bool RpcConnectionPoll::bindRegister(const std::string &host) {
    Address::ptr addr = Address::LookUpAnyIpAddress(host);
    if (!addr) {
        LYON_LOG_ERROR(g_logger) << "RpcServer::bindRegister fail";
        return false;
    }
    return bindRegister(addr);
}

RpcClient::ptr RpcConnectionPoll::getConnection(const RpcMethodMeta &method) {
    std::vector<RpcClient *> invalid_connections;
    RpcClient *rt = nullptr;
    MutexType::Lock lock(m_mutex);
    if (!m_connections.contains(method)) {
        m_connections.emplace(method, std::list<RpcClient *>{});
    }
    while (!m_connections[method].empty()) {
        auto conn = m_connections[method].front();
        m_connections[method].pop_front();
        if (!conn->isConnected()) {
            invalid_connections.push_back(conn);
            continue;
        }

        if (conn->getCreateTime() + m_maxLiveTime >= GetCurrentTimeMS()) {
            invalid_connections.push_back(conn);
            continue;
        }
        rt = conn;
        break;
    }
    lock.unlock();

    for (auto &conn : invalid_connections) {
        delete conn;
    }
    m_total -= invalid_connections.size();

    if (!rt) {
        std::vector<std::string> servers = discover(method);
        LYON_LOG_DEBUG(g_logger) << "discover " << servers[0];
        if (servers.empty()) {
            return nullptr;
        }
        rt = new RpcClient();
        if (!rt->connect(servers[0])) {
            return nullptr;
        }

        ++m_total;
    }

    return RpcClient::ptr(rt, std::bind(RpcConnectionPoll::ReleasePtr,
                                        std::placeholders::_1, this, method));
}

std::vector<std::string>
RpcConnectionPoll::discover(const RpcMethodMeta &method) {
    std::vector<std::string> servers;
    if (!m_registerSession || !m_registerSession->isConnected()) {
        return servers;
    }

    RpcProtocol::ptr request = RpcProtocol::CreateDiscoverMethodRequest();
    Serializer ser;
    ser << method;
    request->setContent(ser.toString());
    m_registerSession->sendRpcProtocol(request);

    RpcProtocol::ptr response = m_registerSession->recvRpcProtocol();

    Serializer result_ser(response->getContent(), response->isCompress());

    result_ser >> servers;
    return servers;
}

void RpcConnectionPoll::ReleasePtr(RpcClient *ptr, RpcConnectionPoll *poll,
                                   RpcMethodMeta method) {
    ptr->incRequest();
    if (!ptr->isConnected() ||
        (ptr->getCreateTime() + poll->m_maxLiveTime >= GetCurrentTimeMS()) ||
        ptr->getRequest() > poll->m_maxRequest ||
        poll->m_total > poll->m_maxSize) {
        delete ptr;
        --poll->m_total;
        return;
    }

    MutexType::Lock lock(poll->m_mutex);
    poll->m_connections[method].push_back(ptr);
}

} // namespace lyon::rpc
