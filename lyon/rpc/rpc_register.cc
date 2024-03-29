#include "rpc_register.h"
#include <cstdint>
#include <iterator>
#include <memory>
namespace lyon::rpc {
static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

bool RpcRegister::aliveTest(const std::string &server) {
    Address::ptr addr = Address::LookUpAnyIpAddress(server);
    Socket::ptr sock = Socket::CreateTCP(addr);
    RpcSession session = RpcSession(sock);

    RpcProtocol::ptr request = RpcProtocol::CreateAliveTestRquest();

    session.sendRpcProtocol(request);

    RpcProtocol::ptr response = session.recvRpcProtocol();
    //如果没有回应，或者连接已经断开
    RWMutexType::WRLock wlock(m_mutex);
    if (!response && !session.isConnected()) {
        auto &methods = m_serverMethod[server];
        for (auto &method : methods) {
            m_registedMethod.erase(method);
        }
        m_serverMethod[server].clear();
        return false;
    }
    return true;
}

void RpcRegister::handleClient(Socket::ptr sock) {
    RpcSession::ptr session = std::make_shared<RpcSession>(sock);
    do {
        RpcProtocol::ptr request = session->recvRpcProtocol();

        if (!request) {
            LYON_LOG_ERROR(g_logger) << "RpcRegister::handleClient Bad Request";
            break;
        }

        RpcProtocol::ptr response;
        if (request->getType() ==
            RpcProtocol::MSG_TYPE::RPC_REGIST_METHOD_REQUEST) {
            response = handleRegistMethod(
                request, session->getSocket()->getRemoteAddress());
        } else if (request->getType() ==
                   RpcProtocol::MSG_TYPE::RPC_DISCOVER_METHOD_REQUEST) {
            response = handleDiscoverMethod(request);
        } else if (request->getType() ==
                   RpcProtocol::MSG_TYPE::RPC_SERVER_ERROR_REQUEST) {
        }

        if (response) {
            session->sendRpcProtocol(response);
        }

        if (!session->isConnected()) {
            break;
        }
    } while (true);
    session->close();
}

RpcProtocol::ptr RpcRegister::handleRegistMethod(RpcProtocol::ptr request,
                                                 Address::ptr server_addr) {
    Serializer request_ser(request->getContent(), request->isCompress());
    RpcProtocol::ptr response =
        RpcProtocol::CreateRegistMethodResponse(request->getSeqId());

    RpcMethodMeta method;
    std::vector<uint16_t> server_ports;
    request_ser >> method >> server_ports;
    // TODO:因为register可能会管理很多服务，所以这可以使用redis存储

    // server开放服务的端口可能不止一个，这里暂时简单处理，使用第一个
    std::static_pointer_cast<IPAddress>(server_addr)->setPort(server_ports[0]);
    // m_registedMethod.emplace(method, std::list{server_addr->toString()});

    RWMutexType::WRLock wlock(m_mutex);
    auto itr = m_registedMethod.emplace(method, server_addr->toString());
    m_serverMethod[server_addr->toString()].push_back(itr);

    LYON_LOG_DEBUG(g_logger) << "regist method: " << method.toString() << " at "
                             << server_addr->toString();

    Serializer result_ser;
    // TODO:这里有个问题，"OK" 并不会视为string被序列化
    std::string str = "OK";
    // result_ser << "OK";
    result_ser << str;
    response->setContent(result_ser.toString());
    return response;
}

RpcProtocol::ptr RpcRegister::handleDiscoverMethod(RpcProtocol::ptr request) {
    Serializer request_ser(request->getContent(), request->isCompress());
    RpcProtocol::ptr response =
        RpcProtocol::CreateDiscoverMethodResponse(request->getSeqId());

    RpcMethodMeta method;
    request_ser >> method;

    Serializer result_ser;
    RWMutexType::RDLock rlock(m_mutex);
    if (!m_registedMethod.contains(method)) {

    } else {
        auto range = m_registedMethod.equal_range(method);
        std::list<std::string> servers;
        for (auto i = range.first; i != range.second; ++i) {
            servers.push_back(i->second);
        }
        result_ser << servers;

        response->setContent(result_ser.toString());
    }

    return response;
}

void RpcRegister::handleServerError(RpcProtocol::ptr request) {
    Serializer request_ser(request->getContent(), request->isCompress());
    RpcProtocol::ptr response =
        RpcProtocol::CreateDiscoverMethodResponse(request->getSeqId());

    std::string server;
    request_ser >> server;

    m_ioworker->addJob([this, server]() { aliveTest(server); });
}

} // namespace lyon::rpc
