#include "rpc_connection.h"
#include "lyon/log.h"
#include "lyon/socket.h"
#include "lyon/util.h"
namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

RpcConnection::RpcConnection(uint64_t timeoutMs) : m_timeoutMs(timeoutMs) {
    m_createTime = GetCurrentTimeMS();
    m_request = 0;
}

RpcConnection::~RpcConnection() {
    if (m_session && m_session->isConnected()) {
        m_session->close();
    }
    LYON_LOG_INFO(g_logger) << "RpcConnection close";
}

bool RpcConnection::connect(Address::ptr addr) {
    Socket::ptr sock = Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        LYON_LOG_ERROR(g_logger) << "RpcConnection::connect fail";
        m_session = nullptr;
        return false;
    }
    sock->setRecvTimeout(m_timeoutMs);
    m_session = std::make_shared<RpcSession>(sock);
    return true;
}

bool RpcConnection::connect(const std::string &host) {
    Address::ptr addr = Address::LookUpAnyIpAddress(host);
    if (!addr) {
        LYON_LOG_ERROR(g_logger) << "RpcConnection::lookup IP fail";
        return false;
    }
    return connect(addr);
}

bool RpcConnection::isConnected() {
    return (m_session && m_session->isConnected());
}

uint64_t RpcConnection::getCreateTime() { return m_createTime; }
} // namespace lyon::rpc
