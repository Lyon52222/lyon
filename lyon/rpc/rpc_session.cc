#include "rpc_session.h"
#include <cstdint>
namespace lyon::rpc {

RpcProtocol::ptr RpcSession::recvRpcProtocol() {
    RpcProtocol::ptr protocol(new RpcProtocol());
    ByteArray::ptr ba(new ByteArray(RpcProtocol::HEAD_LEN));
    int rt = read(ba, RpcProtocol::HEAD_LEN);
    if (rt <= 0) {
        return nullptr;
    }
    protocol->parserHead(ba);

    if (!protocol->isValid()) {
        return nullptr;
    }
    uint32_t content_len = protocol->getContentLen();
    if (content_len > 0) {
        std::string content;
        content.resize(content_len);
        rt = read(&content[0], content_len);
        if (rt <= 0) {
            return nullptr;
        }
        protocol->setContent(content);
    }

    return protocol;
}

int RpcSession::sendRpcProtocol(RpcProtocol::ptr protocol) {
    ByteArray::ptr ba = protocol->serialize();
    return write(ba, ba->getReadableSize());
}

} // namespace lyon::rpc
