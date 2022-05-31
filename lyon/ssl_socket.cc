#include "ssl_socket.h"
#include "log.h"
#include "macro.h"
#include "socket.h"
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <sys/types.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

namespace {
struct _SSLIniter {
    _SSLIniter() {
#if OPENSSL_VERSION_NUMBER < 0x1010001fL
        SSL_load_error_strings();
        SSL_library_init();
#else
        OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS |
                             OPENSSL_INIT_LOAD_CRYPTO_STRINGS,
                         NULL);
#endif
    }
};
static _SSLIniter ssliniter;
} // namespace

SSLSocket::SSLSocket(int type, int family, int protocol)
    : Socket(type, family, protocol) {}

SSLSocket::ptr SSLSocket::CreateTCP(Address::ptr address) {
    return SSLSocket::ptr(new SSLSocket(TCP, address->getFamily(), 0));
}

SSLSocket::ptr SSLSocket::CreateTCPSocket() {
    return SSLSocket::ptr(new SSLSocket(TCP, IPv4, 0));
}

SSLSocket::ptr SSLSocket::CreateTCPSocket6() {
    return SSLSocket::ptr(new SSLSocket(TCP, IPv6, 0));
}

bool SSLSocket::init(int socket) {
    bool rt = Socket::init(socket);
    if (rt) {
        m_ssl.reset(SSL_new(m_ctx.get()), SSL_free);
        SSL_set_fd(m_ssl.get(), m_socket);
        rt = (SSL_accept(m_ssl.get()) == 1);
    }
    return rt;
}

bool SSLSocket::bind(Address::ptr address) { return Socket::bind(address); }

bool SSLSocket::listen(int backlog) { return Socket::listen(backlog); }

Socket::ptr SSLSocket::accept() {
    SSLSocket::ptr sslsock(new SSLSocket(m_type, m_family, m_protocol));
    int newsock = ::accept(m_socket, nullptr, nullptr);
    if (newsock == -1) {
        LYON_LOG_ERROR(g_logger) << "SSLSocket::accept fail";
        return nullptr;
    }
    if (sslsock->init(newsock)) {
        return sslsock;
    }
    return nullptr;
}

bool SSLSocket::connect(Address::ptr address, uint64_t timeout_ms) {
    bool rt = Socket::connect(address, timeout_ms);
    if (rt) {
        m_ctx.reset(SSL_CTX_new(SSLv23_client_method()), SSL_CTX_free);
        m_ssl.reset(SSL_new(m_ctx.get()), SSL_free);
        SSL_set_fd(m_ssl.get(), m_socket);
        rt = (SSL_connect(m_ssl.get()) == 1);
    }
    return rt;
}

bool SSLSocket::close() { return Socket::close(); }

ssize_t SSLSocket::recv(void *buffer, size_t length, int flags) {
    if (m_ssl) {
        return SSL_read(m_ssl.get(), buffer, length);
    }
    return -1;
}

ssize_t SSLSocket::recv(iovec *buffers, size_t length, int flags) {
    if (!m_ssl) {
        return -1;
    }
    ssize_t size = 0;
    for (size_t i = 0; i < length; i++) {
        size_t rt =
            SSL_read(m_ssl.get(), buffers[i].iov_base, buffers[i].iov_len);
        if (rt <= 0) {
            return rt;
        }
        size += rt;
        if (rt < buffers[i].iov_len) {
            break;
        }
    }
    return size;
}

ssize_t SSLSocket::recvFrom(Address::ptr address, void *buffer, size_t length,
                            int flags) {
    LYON_ASSERT(false);
}

ssize_t SSLSocket::recvFrom(Address::ptr address, iovec *buffers, size_t length,
                            int flags) {
    LYON_ASSERT(false);
}

ssize_t SSLSocket::send(const void *buffer, size_t length, int flags) {
    if (m_ssl) {
        return SSL_write(m_ssl.get(), buffer, length);
    }
    return -1;
}

ssize_t SSLSocket::send(const iovec *buffers, size_t length, int flags) {
    if (!m_ssl) {
        return -1;
    }
    ssize_t size = 0;
    for (size_t i = 0; i < length; i++) {
        size_t rt =
            SSL_write(m_ssl.get(), buffers[i].iov_base, buffers[i].iov_len);
        if (rt <= 0) {
            return -1;
        }
        size += rt;
        if (rt < buffers[i].iov_len) {
            break;
        }
    }
    return size;
}

ssize_t SSLSocket::sendTo(Address::ptr address, const void *buffer,
                          size_t length, int flags) {
    LYON_ASSERT(false);
}

ssize_t SSLSocket::sendTo(Address::ptr address, const iovec *buffers,
                          size_t length, int flags) {
    LYON_ASSERT(false);
}

} // namespace lyon
