#include "rpc_server.h"
#include "rpc_method.h"
#include "rpc_protocol.h"
#include "rpc_result.h"
#include "rpc_session.h"
namespace lyon::rpc {

RpcServer::RpcServer(IOManager *worker, IOManager *ioworker,
                     IOManager *acceptWorker)
    : TcpServer(worker, ioworker, acceptWorker) {}

void RpcServer::handleClient(Socket::ptr sock) {
    RpcSession::ptr session(new RpcSession(sock));

    RpcProtocol::ptr request = session->recvRpcProtocol();
    RpcProtocol::ptr response;

    if (request->getType() == RpcProtocol::MSG_TYPE::RPC_METHOD_REQUEST) {
        response = handleMethodRequest(request);
    }

    if (response)
        session->sendRpcProtocol(response);
}

RpcProtocol::ptr RpcServer::handleMethodRequest(RpcProtocol::ptr request) {
    Serializer content(request->getContent(), request->isCompress());
    std::string method_name;
    content >> method_name;

    RpcProtocol::ptr response = RpcProtocol::CreateMethodResponse();
    response->setSeqId(request->getSeqId());

    // RpcResult<>::ptr result;
    std::unique_ptr<RpcResult<>> result;
    if (!m_methods.contains(method_name)) {
        result = std::make_unique<RpcResult<>>(RpcResultState::METHOD_NOT_FOUND,
                                               "method not found");
    } else {
        if (m_methods[method_name]->call(content)) {
            result = std::make_unique<RpcResult<>>(RpcResultState::OK, "OK");
            // TODO:这里要考虑如何将返回值直接存放为可以使用Serializer反序列化的数据
        } else {
            result = std::make_unique<RpcResult<>>(
                RpcResultState::METHOD_RUN_ERROR, "method run error");
        }
    }
    response->setContent(result->toString());
    return response;
}

void RpcServer::registMethod(RPCMethod::ptr method) {
    m_methods.emplace(method->getName(), method);
}

RPCMethod::ptr RpcServer::getMethod(const std::string &name) {
    auto itr = m_methods.find(name);
    if (itr == m_methods.end()) {
        return nullptr;
    }
    return itr->second;
}

} // namespace lyon::rpc
