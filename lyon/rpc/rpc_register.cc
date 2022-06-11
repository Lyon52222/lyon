#include "rpc_register.h"
#include <lyon/log.h>
#include <lyon/serialize/serializer.h>
#include <memory>
#include <rpc_protocol.h>
#include <rpc_session.h>
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
            response = handleRegistMethod(request);
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

RpcProtocol::ptr RpcRegister::handleRegistMethod(RpcProtocol::ptr request) {
    Serializer info(request->getContent(), request->isCompress());
    RpcProtocol::ptr response =
        RpcProtocol::CreateRegistMethodResponse(request->getSeqId());
    // TODO:设置对应的响应体

    return response;
}

} // namespace lyon::rpc
