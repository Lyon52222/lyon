#include "rpc_server.h"
#include "rpc_method.h"
#include "rpc_protocol.h"
#include "rpc_result.h"
#include "rpc_session.h"
#include <lyon/log.h>
namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

RpcServer::RpcServer(IOManager *worker, IOManager *ioworker,
                     IOManager *acceptWorker)
    : TcpServer(worker, ioworker, acceptWorker) {}

void RpcServer::handleClient(Socket::ptr sock) {
    //新建一个session
    RpcSession::ptr session(new RpcSession(sock));

    //重复收发消息
    do {
        //接收请求
        RpcProtocol::ptr request = session->recvRpcProtocol();
        //如果请求接收或解析失败
        if (!request) {
            LYON_LOG_ERROR(g_logger) << "RpcServer::handleClient Bad Request";
            // continue;
            break;
        }
        LYON_LOG_DEBUG(g_logger)
            << "Server recv Request = " << request->toString();
        RpcProtocol::ptr response;
        if (request->getType() == RpcProtocol::MSG_TYPE::RPC_METHOD_REQUEST) {
            //如果是函数调用请求
            response = handleMethodRequest(request);
        }
        LYON_LOG_DEBUG(g_logger)
            << "Server send Response = " << response->toString();

        if (response) {
            session->sendRpcProtocol(response);
        }
        // TODO:这里暂时假设所有请求都是短连接
        if (true || !session->isConnected()) {
            break;
        }
    } while (true);
    session->close();
}

RpcProtocol::ptr RpcServer::handleMethodRequest(RpcProtocol::ptr request) {
    //先将消息体转化为序列化参数
    Serializer args(request->getContent(), request->isCompress());
    std::string method_name;
    //读取函数名
    args >> method_name;

    //创建对应的响应
    RpcProtocol::ptr response =
        RpcProtocol::CreateMethodResponse(request->getSeqId());

    //响应结果的封装
    std::unique_ptr<RpcResult<>> result;

    if (!m_methods.contains(method_name)) {
        result = std::make_unique<RpcResult<>>(RpcResultState::METHOD_NOT_FOUND,
                                               "method not found");
    } else {
        //调用函数
        if (m_methods[method_name]->call(args)) {
            result = std::make_unique<RpcResult<>>(RpcResultState::OK, "OK");
            //将处理结果的封装和函数的返回值放入响应体中
            response->setContent(result->toString() + args.toString());
            return response;
        } else {
            result = std::make_unique<RpcResult<>>(
                RpcResultState::METHOD_RUN_ERROR, "method run error");
        }
    }
    //其余调用失败的的时候，就不需要仿佛处理结果了。
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
