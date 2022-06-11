#include "rpc_session.h"
#include "rpc_protocol.h"
#include <cstdint>
#include <lyon/log.h>
namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

RpcProtocol::ptr RpcSession::recvRpcProtocol() {
    RpcProtocol::ptr protocol(new RpcProtocol());
    //先接受消息头
    ByteArray::ptr ba(new ByteArray());
    LYON_LOG_DEBUG(g_logger) << "start read head";
    ssize_t rt = readFixSize(ba, RpcProtocol::HEAD_LEN);
    LYON_LOG_DEBUG(g_logger) << "read head rt = " << rt;
    if (rt <= 0) {
        LYON_LOG_DEBUG(g_logger) << "RpcSession::recvRpcProtocol read rt=" << rt
                                 << " error = " << strerror(errno);
        //如果对方已经关闭连接(rt==0) 或接收错误(rt <0);
        //关闭socket
        close();
        return nullptr;
    }

    //解析消息头中的信息
    ba->setPosition(0);
    protocol->parserHead(ba);

    //判断消息是否有效
    if (!protocol->isValid()) {
        return nullptr;
    }

    //获得消息体的长度
    uint32_t content_len = protocol->getContentLen();

    if (content_len > 0) {
        //接收消息体
        std::string content;
        content.resize(content_len);
        rt = readFixSize(&content[0], content_len);
        if (rt <= 0) {
            close();
            return nullptr;
        }
        //设置消息体
        protocol->setContent(content);
    }

    return protocol;
}

int RpcSession::sendRpcProtocol(RpcProtocol::ptr protocol) {
    ByteArray::ptr ba = protocol->serialize();
    ba->setPosition(0);
    return writeFixSize(ba, ba->getReadableSize());
}

} // namespace lyon::rpc
