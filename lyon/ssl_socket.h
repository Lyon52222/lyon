#ifndef __LYON_SSH_SOCKET_H__
#define __LYON_SSH_SOCKET_H__
#include "socket.h"
namespace lyon {

class SSLSocket : public Socket {
public:
    typedef std::shared_ptr<SSLSocket> ptr;

private:
};

} // namespace lyon

#endif
