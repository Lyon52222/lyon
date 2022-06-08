#include "rpc_client.h"
#include "lyon/log.h"
#include "lyon/socket.h"
namespace lyon::rpc {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

bool RpcClient::connect(Address::ptr addr) {
    Socket::ptr sock = Socket::CreateTCP(addr);
    if (!sock->connect(addr)) {
        LYON_LOG_INFO(g_logger) << "RPCClient::connect fail";
        return false;
    }
    sock->setRecvTimeout(m_timeoutMs);
    m_session = std::make_shared<RpcSession>(sock);
    return true;
}

bool RpcClient::connect(const std::string &host) {
    Address::ptr addr = Address::LookUpAnyIpAddress(host);
    if (!addr) {
        LYON_LOG_INFO(g_logger) << "RPCClient::connect fail";
        return false;
    }
    return connect(addr);
}
} // namespace lyon::rpc
