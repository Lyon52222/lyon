#include "rpc_client.h"
#include "lyon/log.h"
#include "lyon/socket.h"
#include "lyon/util.h"
namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

RpcClient::RpcClient(uint64_t timeoutMs) : m_timeoutMs(timeoutMs) {
    m_createTime = GetCurrentTimeMS();
    m_request = 0;
}

RpcClient::~RpcClient() {
    if (m_session && m_session->isConnected()) {
        m_session->close();
    }
    LYON_LOG_INFO(g_logger) << "RpcClient close";
}

bool RpcClient::connect(Address::ptr addr) {
    Socket::ptr sock = Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        LYON_LOG_ERROR(g_logger) << "RpcClient::connect fail";
        m_session = nullptr;
        return false;
    }
    sock->setRecvTimeout(m_timeoutMs);
    m_session = std::make_shared<RpcSession>(sock);
    return true;
}

bool RpcClient::connect(const std::string &host) {
    Address::ptr addr = Address::LookUpAnyIpAddress(host);
    if (!addr) {
        LYON_LOG_ERROR(g_logger) << "RpcClient::lookup IP fail";
        return false;
    }
    return connect(addr);
}

bool RpcClient::isConnected() {
    return (m_session && m_session->isConnected());
}

uint64_t RpcClient::getCreateTime() { return m_createTime; }
} // namespace lyon::rpc
