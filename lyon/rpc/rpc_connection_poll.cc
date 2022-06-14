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

RpcConnection::ptr
RpcConnectionPoll::getConnection(const RpcMethodMeta &method) {
    std::vector<RpcConnection *> invalid_connections;
    RpcConnection *rt = nullptr;
    MutexType::Lock lock(m_mutex);
    if (!m_connections.contains(method)) {
        m_connections.emplace(method, std::list<RpcConnection *>{});
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
        rt = new RpcConnection();
        std::list<std::string> &servers = m_servers[method];
        //现在已知的服务器中尝试
        while (!servers.empty()) {
            std::string &server = servers.front();
            //将请求失败的删除
            if (!rt->connect(server)) {
                servers.pop_front();
                // TODO:这里应该通知register 该服务已失效
                continue;
            } else {
                break;
            }
        }
        //如果尝试完了都没成功
        if (!rt->isConnected()) {
            //想register发现服务器
            std::vector<std::string> discovered_servers = discover(method);
            servers.insert(servers.begin(), discovered_servers.begin(),
                           discovered_servers.end());
            while (!servers.empty()) {
                std::string &server = servers.front();
                //将请求失败的删除
                if (!rt->connect(server)) {
                    servers.pop_front();
                    continue;
                } else {
                    break;
                }
            }
        }

        if (!rt->isConnected()) {
            return nullptr;
        }

        ++m_total;
    }

    return RpcConnection::ptr(rt,
                              std::bind(RpcConnectionPoll::ReleasePtr,
                                        std::placeholders::_1, this, method));
}

std::vector<std::string>
RpcConnectionPoll::discover(const RpcMethodMeta &method) {
    std::vector<std::string> servers;
    if (!m_registerSession || !m_registerSession->isConnected()) {
        LYON_LOG_INFO(g_logger) << "RpcConnectionPoll not bind Register";
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

void RpcConnectionPoll::ReleasePtr(RpcConnection *ptr, RpcConnectionPoll *poll,
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
