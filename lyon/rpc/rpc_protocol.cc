#include "rpc_protocol.h"
#include <cstdint>
namespace lyon::rpc {

std::atomic<uint64_t> RpcProtocol::m_id = {0};

RpcProtocol::RpcProtocol() {}

RpcProtocol::RpcProtocol(MSG_TYPE type, uint8_t flag) : m_flag(flag) {
    m_type = static_cast<uint8_t>(type);
}

void RpcProtocol::parserHead(ByteArray::ptr ba) {
    m_magic = ba->readFuint8();
    m_version = ba->readFuint8();
    m_type = ba->readFuint8();
    m_flag = ba->readFuint8();
    m_seqId = ba->readFuint32();
    m_contentLen = ba->readFuint32();
}

ByteArray::ptr RpcProtocol::serialize() const {
    ByteArray::ptr ba = std::make_shared<ByteArray>();
    ba->writeFuint8(m_magic);
    ba->writeFuint8(m_version);
    ba->writeFuint8(m_type);
    ba->writeFuint8(m_flag);
    ba->writeFuint32(m_seqId);
    ba->writeFuint32(m_contentLen);
    ba->writeStringRaw(m_content);

    return ba;
}

bool RpcProtocol::isValid() const {

    if (m_magic != MAGIC || m_version != VERSION) {
        return false;
    }
    return true;
}

RpcProtocol::ptr RpcProtocol::CreateMethodRequest() {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_METHOD_REQUEST, 0x01));
    protocol->setSeqId(++m_id);
    return protocol;
}
RpcProtocol::ptr RpcProtocol::CreateMethodResponse() {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_METHOD_REQUEST, 0x01));
    return protocol;
}

} // namespace lyon::rpc
