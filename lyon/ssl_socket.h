#ifndef __LYON_SSH_SOCKET_H__
#define __LYON_SSH_SOCKET_H__
#include "address.h"
#include "socket.h"
#include <memory>
#include <openssl/ssl.h>
namespace lyon {

class SSLSocket : public Socket {
public:
    typedef std::shared_ptr<SSLSocket> ptr;

    SSLSocket(int type, int family, int protocol = 0);

    static SSLSocket::ptr CreateTCP(Address::ptr address);
    static SSLSocket::ptr CreateTCPSocket();
    static SSLSocket::ptr CreateTCPSocket6();

    virtual bool init(int socket) override;
    virtual bool bind(Address::ptr address) override;
    virtual bool listen(int backlog = SOMAXCONN) override;
    virtual Socket::ptr accept() override;
    virtual bool connect(Address::ptr address,
                         uint64_t timeout_ms = -1) override;

    virtual bool close() override;

    virtual ssize_t recv(void *buffer, size_t length, int flags = 0) override;

    virtual ssize_t recv(iovec *buffers, size_t length, int flags = 0) override;

    virtual ssize_t recvFrom(Address::ptr address, void *buffer, size_t length,
                             int flags = 0) override;

    virtual ssize_t recvFrom(Address::ptr address, iovec *buffers,
                             size_t length, int flags = 0) override;

    virtual ssize_t send(const void *buffer, size_t length,
                         int flags = 0) override;

    virtual ssize_t send(const iovec *buffers, size_t length,
                         int flags = 0) override;

    virtual ssize_t sendTo(Address::ptr address, const void *buffer,
                           size_t length, int flags = 0) override;

    virtual ssize_t sendTo(Address::ptr address, const iovec *buffers,
                           size_t length, int flags = 0) override;

private:
    std::shared_ptr<SSL_CTX> m_ctx;
    std::shared_ptr<SSL> m_ssl;
};

} // namespace lyon

#endif
