#include "rpc_session.h"
#include <cstdint>
#include <lyon/serialize/bytearray.h>
#include <lyon/serialize/serializer.h>
#include <rpc_protocol.h>
namespace lyon::rpc {

RPCProtocol::ptr RPCSession::recvRPCProtocol() {
    RPCProtocol::ptr protocol(new RPCProtocol());
    ByteArray::ptr ba(new ByteArray(RPCProtocol::HEAD_LEN));
    int rt = read(ba, RPCProtocol::HEAD_LEN);
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

int RPCSession::sendRPCProtocol(RPCProtocol::ptr protocol) {
    ByteArray::ptr ba = protocol->serialize();
    return write(ba, ba->getReadableSize());
}

} // namespace lyon::rpc
