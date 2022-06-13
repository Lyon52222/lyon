#ifndef __LYON_RPC_SESSION_H__
#define __LYON_RPC_SESSION_H__

#include "lyon/socket.h"
#include "lyon/streams/socket_stream.h"
#include "rpc_protocol.h"
#include <cstdint>
#include <memory>

namespace lyon::rpc {

class RpcSession : public SocketStream {
public:
    typedef std::shared_ptr<RpcSession> ptr;

    RpcSession(Socket::ptr sock) : SocketStream(sock) {}
    RpcProtocol::ptr recvRpcProtocol();
    int sendRpcProtocol(RpcProtocol::ptr protocol);

private:
    void setTimeout(uint64_t timeout);
};

} // namespace lyon::rpc

#endif
