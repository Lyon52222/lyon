#include "socket.h"
#include "log.h"
#include <address.h>
#include <asm-generic/socket.h>
#include <macro.h>
#include <memory>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <utils/file_system_util.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

Socket::Socket(int type, int family, int protocol)
    : m_socket(-1), m_type(Type(type)), m_family(Family(family)),
      m_protocol(protocol), m_isConnect(false) {}

Socket::~Socket() { close(); }

Socket::ptr Socket::CreateTCP(Address::ptr address) {
    Socket::ptr socket(new Socket(TCP, address->getFamily(), 0));
    return socket;
}
Socket::ptr Socket::CreateUDP(Address::ptr address) {
    Socket::ptr socket(new Socket(UDP, address->getFamily(), 0));
    return socket;
}

Socket::ptr Socket::CreateTCPSocket() {
    Socket::ptr socket(new Socket(TCP, IPv4, 0));
    return socket;
}
Socket::ptr Socket::CreateTCPSocket6() {
    Socket::ptr socket(new Socket(TCP, IPv6, 0));
    return socket;
}
Socket::ptr Socket::CreateTCPUnixSocket() {
    Socket::ptr socket(new Socket(TCP, Unix, 0));
    return socket;
}

Socket::ptr Socket::CreateUDPSocket() {
    Socket::ptr socket(new Socket(UDP, IPv4, 0));
    return socket;
}
Socket::ptr Socket::CreateUDPSocket6() {
    Socket::ptr socket(new Socket(UDP, IPv6, 0));
    return socket;
}
Socket::ptr Socket::CreateTCPUnixSocket6() {
    Socket::ptr socket(new Socket(UDP, Unix, 0));
    return socket;
}

int Socket::setOption(int level, int opt_name, const void *opt_value,
                      socklen_t opt_len) {
    if (setsockopt(m_socket, level, opt_name, opt_value, opt_len)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::setOption ( " << m_socket << ", " << level << ", "
            << opt_name << ", " << opt_value << ", " << opt_len;
        return false;
    }
    return true;
}

int Socket::getOption(int level, int opt_name, void *opt_value,
                      socklen_t *opt_len) {
    if (getsockopt(m_socket, level, opt_name, opt_value, opt_len)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::getOption ( " << m_socket << ", " << level << ", "
            << opt_name << ", " << opt_value << ", " << opt_len;
        return false;
    }
    return true;
}

bool Socket::isValid() const { return m_socket != -1; }

void Socket::newSocket() {
    m_socket = socket(m_family, m_type, m_protocol);
    if (LYON_UNLIKELY(m_socket == -1)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::newSock error : ( " << m_family << ", " << m_type
            << ", " << m_protocol << ") rt = " << m_socket
            << " errno = " << errno;
    } else {
        initSocket();
    }
}

void Socket::initSocket() {
    int val = 1;
    setOption(SOL_SOCKET, SO_REUSEADDR, val);
    if (m_type == TCP) {
        setOption(IPPROTO_TCP, TCP_NODELAY, val);
    }
}

bool Socket::bind(Address::ptr address) {
    if (!isValid()) {
        newSocket();
        if (LYON_UNLIKELY(!isValid())) {
            return false;
        }
    }

    if (LYON_UNLIKELY(m_family != address->getFamily())) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::bind family not equal. m_family = " << m_family
            << " address family = " << address->getFamily();
    }

    UnixAddress::ptr unixaddr = std::dynamic_pointer_cast<UnixAddress>(address);
    if (unixaddr) {
        Socket::ptr sock = CreateTCPUnixSocket();
        if (sock->connect(address)) {
            return false;
        } else {
            FSUtil::Unlink(unixaddr->getPath(), true);
        }
    }

    if (::bind(m_socket, address->getAddr(), address->getAddrLen())) {
        LYON_LOG_ERROR(g_logger) << "Socket::bind sock : " << m_socket
                                 << " address : " << address->toString();
        return false;
    }
    return true;
}
bool Socket::listen(int backlog) {
    if (!isValid()) {
        LYON_LOG_ERROR(g_logger) << "Socket::listen sockt = -1";
        return false;
    }
    if (::listen(m_socket, backlog)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::listen error = " << strerror(errno);
        return false;
    }
    return true;
}
int Socket::accept(Address::ptr address);
int Socket::connect(Address::ptr address);

bool Socket::close() {
    if (!m_isConnect && m_socket == -1) {
        return true;
    }
    m_isConnect = false;
    if (m_socket != -1) {
        ::close(m_socket);
        m_socket = -1;
    }
    return false;
}

} // namespace lyon
