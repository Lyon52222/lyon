#include "rpc_server.h"
#include "rpc_method.h"
#include <rpc_protocol.h>
#include <rpc_session.h>
namespace lyon::rpc {

RPCServer::RPCServer(IOManager *worker, IOManager *ioworker,
                     IOManager *acceptWorker)
    : TcpServer(worker, ioworker, acceptWorker) {}

void RPCServer::handleClient(Socket::ptr sock) {
    RPCSession::ptr session(new RPCSession(sock));

    RPCProtocol::ptr request = session->recvRPCProtocol();
    RPCProtocol::ptr response;

    if (request->getType() == RPCProtocol::MSG_TYPE::RPC_METHOD_REQUEST) {
        response = handleMethodRequest(request);
    }

    if (response)
        session->sendRPCProtocol(response);
}

RPCProtocol::ptr RPCServer::handleMethodRequest(RPCProtocol::ptr request) {
    Serializer content(request->getContent(), request->isFix());
    std::string method_name;
    content >> method_name;

    RPCProtocol::ptr response = RPCProtocol::CreateMethodResponse();
    response->setSeqId(request->getSeqId());
    if (!m_methods.contains(method_name)) {
        response->setContent("method not found");
    } else {
        if (m_methods[method_name]->call(content)) {
            response->setContent(content.toString());
        } else {
            response->setContent("call method error");
        }
    }
    return response;
}

void RPCServer::registMethod(RPCMethod::ptr method) {
    m_methods.emplace(method->getName(), method);
}

RPCMethod::ptr RPCServer::getMethod(const std::string &name) {
    auto itr = m_methods.find(name);
    if (itr == m_methods.end()) {
        return nullptr;
    }
    return itr->second;
}

} // namespace lyon::rpc
