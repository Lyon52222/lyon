#ifndef __LYON_RPC_SESSION_H__
#define __LYON_RPC_SESSION_H__

#include "lyon/socket.h"
#include "lyon/streams/socket_stream.h"
#include "rpc_protocol.h"
#include <memory>

namespace lyon::rpc {

class RPCSession : public SocketStream {
public:
    typedef std::shared_ptr<RPCSession> ptr;

    RPCSession(Socket::ptr sock) : SocketStream(sock) {}
    RPCProtocol::ptr recvRPCProtocol();
    int sendRPCProtocol(RPCProtocol::ptr protocol);
};

} // namespace lyon::rpc

#endif
