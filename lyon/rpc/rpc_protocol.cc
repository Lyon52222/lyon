#include "rpc_protocol.h"
#include <cstdint>
#include <sstream>
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

void RpcProtocol::setContent(const std::string &content) {
    m_content = content;
    m_contentLen = m_content.size();
}

std::ostream &RpcProtocol::dump(std::ostream &os) const {
    os << "[" << static_cast<unsigned>(m_magic) << "|"
       << static_cast<unsigned>(m_version) << "|"
       << static_cast<unsigned>(m_type) << "|" << static_cast<unsigned>(m_flag)
       << "|" << m_seqId << "|" << m_contentLen << "|" << m_content << "]";
    return os;
}
std::string RpcProtocol::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

RpcProtocol::ptr RpcProtocol::CreateCallMethodRequest() {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_CALL_METHOD_REQUEST, 0x01));
    protocol->setSeqId(++m_id);
    return protocol;
}

RpcProtocol::ptr RpcProtocol::CreateCallMethodResponse(uint32_t seq_id) {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_CALL_METHOD_RESPONSE, 0x01));
    protocol->setSeqId(seq_id);
    return protocol;
}

RpcProtocol::ptr RpcProtocol::CreateRegistMethodRequest() {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_REGIST_METHOD_REQUEST, 0x01));
    protocol->setSeqId(++m_id);
    return protocol;
}

RpcProtocol::ptr RpcProtocol::CreateRegistMethodResponse(uint32_t seq_id) {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_REGIST_METHOD_RESPONSE, 0x01));
    protocol->setSeqId(seq_id);
    return protocol;
}

RpcProtocol::ptr RpcProtocol::CreateDiscoverMethodRequest() {
    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_DISCOVER_METHOD_REQUEST, 0x01));
    protocol->setSeqId(++m_id);
    return protocol;
}

RpcProtocol::ptr RpcProtocol::CreateDiscoverMethodResponse(uint32_t seq_id) {

    RpcProtocol::ptr protocol(
        new RpcProtocol(MSG_TYPE::RPC_DISCOVER_METHOD_RESPONSE, 0x01));
    protocol->setSeqId(seq_id);
    return protocol;
}

} // namespace lyon::rpc
