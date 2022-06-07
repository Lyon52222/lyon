#include "rpc_protocol.h"
namespace lyon::rpc {

RPCProtocol::RPCProtocol() {}

RPCProtocol::RPCProtocol(MSG_TYPE type, uint8_t flag) : m_flag(flag) {
    m_type = static_cast<uint8_t>(type);
}

void RPCProtocol::parserHead(ByteArray::ptr ba) {
    m_magic = ba->readFuint8();
    m_version = ba->readFuint8();
    m_type = ba->readFuint8();
    m_flag = ba->readFuint8();
    m_seqId = ba->readFuint32();
    m_contentLen = ba->readFuint32();
}

ByteArray::ptr RPCProtocol::serialize() const {
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

bool RPCProtocol::isValid() const {

    if (m_magic != MAGIC || m_version != VERSION) {
        return false;
    }
    return true;
}

RPCProtocol::ptr RPCProtocol::CreateMethodRequest() {
    RPCProtocol::ptr protocol(
        new RPCProtocol(MSG_TYPE::RPC_METHOD_REQUEST, 0x01));
    protocol->setSeqId(++m_id);
    return protocol;
}
RPCProtocol::ptr RPCProtocol::CreateMethodResponse() {
    RPCProtocol::ptr protocol(
        new RPCProtocol(MSG_TYPE::RPC_METHOD_REQUEST, 0x01));
    return protocol;
}

} // namespace lyon::rpc
