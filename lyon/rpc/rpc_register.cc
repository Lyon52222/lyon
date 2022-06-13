#include "rpc_register.h"
#include <lyon/address.h>
#include <lyon/log.h>
#include <lyon/serialize/serializer.h>
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
                request, session->getSocket()->getRemoteAddress());
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
    request_ser >> method;

    LYON_LOG_DEBUG(g_logger) << "regist method: " << method.toString();

    // m_registedMethod.push_back(method);
    m_registedMethod.emplace(method, server_addr->toString());

    Serializer result_ser;

    // TODO:这里有个问题，"OK" 并不会视为string被序列化
    std::string str = "OK";

    // result_ser << "OK";
    result_ser << str;

    response->setContent(result_ser.toString());

    return response;
}

RpcProtocol::ptr RpcRegister::handleDiscoverMethod(RpcProtocol::ptr request) {
    RpcProtocol::ptr response =
        RpcProtocol::CreateDiscoverMethodResponse(request->getSeqId());

    return response;
}

} // namespace lyon::rpc
