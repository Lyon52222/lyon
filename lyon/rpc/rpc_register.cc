#include "rpc_register.h"
#include <cstdint>
#include <lyon/address.h>
#include <memory>
namespace lyon::rpc {
static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

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
                // TODO:这里获得的remoteaddr的port并不是提供服务的端口号
                request, session->getSocket()->getRemoteAddress());
        } else if (request->getType() ==
                   RpcProtocol::MSG_TYPE::RPC_DISCOVER_METHOD_REQUEST) {
            response = handleDiscoverMethod(request);
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

    // server开放服务的端口可能不止一个，这里暂时简单处理，使用第一个
    std::static_pointer_cast<IPAddress>(server_addr)->setPort(server_ports[0]);
    if (m_registedMethod.contains(method)) {
        m_registedMethod[method].push_back(server_addr->toString());
    } else {
        m_registedMethod.emplace(method, std::list{server_addr->toString()});
    }

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
    if (!m_registedMethod.contains(method)) {

    } else {
        result_ser << (m_registedMethod[method]);

        response->setContent(result_ser.toString());
    }

    return response;
}

} // namespace lyon::rpc
