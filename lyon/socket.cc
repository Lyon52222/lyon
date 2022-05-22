#include "socket.h"
#include "fdmanager.h"
#include "hook.h"
#include "iomanager.h"
#include "log.h"
#include "macro.h"
#include "utils/file_system_util.h"
#include <asm-generic/socket.h>
#include <cstdint>
#include <memory>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/time.h>
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

uint64_t Socket::getRecvTimeout() const {
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_socket);
    if (ctx) {
        return ctx->getTimeout(SO_RCVTIMEO);
    }
    return -1;
}

void Socket::setRecvTimeout(uint64_t timeout) {
    // c11
    // 新特性，默认构造函数可以使用大括号初始化，只要顺序和类成员变量顺序一致就行
    struct timeval tv {
        int(timeout / 1000), int(timeout % 1000 * 1000)
    };
    setOption(SOL_SOCKET, SO_RCVTIMEO, tv);
}

uint64_t Socket::getSendTimeout() const {
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_socket);
    if (ctx) {
        return ctx->getTimeout(SO_SNDTIMEO);
    }
    return -1;
}

void Socket::setSendTimeout(uint64_t timeout) {
    struct timeval tv {
        int(timeout / 1000), int(timeout % 1000 * 1000)
    };
    setOption(SOL_SOCKET, SO_SNDTIMEO, tv);
}

Address::ptr Socket::getLocalAddress() {
    if (m_localAddress) {
        return m_localAddress;
    }
    Address::ptr localaddr;
    switch (m_family) {
    case AF_INET: {
        localaddr.reset(new IPv4Address());
    } break;
    case AF_INET6: {
        localaddr.reset(new IPv6Address());
    } break;
    case AF_UNIX: {
        localaddr.reset(new UnixAddress());
    } break;
    default: {
        localaddr.reset(new UnKnownAddress(m_family));
    }
    }
    socklen_t addrlen = localaddr->getAddrLen();
    if (getsockname(m_socket, localaddr->getAddr(), &addrlen)) {
        LYON_LOG_ERROR(g_logger)
            << "getsocketname error addr = " << localaddr->toString()
            << " error = " << strerror(errno);
    }

    if (m_family == AF_UNIX) {
        std::dynamic_pointer_cast<UnixAddress>(localaddr)->setAddrLen(addrlen);
    }

    m_localAddress = localaddr;
    return m_localAddress;
}

Address::ptr Socket::getRemoteAddress() {
    if (m_remoteAddress) {
        return m_remoteAddress;
    }
    Address::ptr remoteaddr;
    switch (m_family) {
    case AF_INET: {
        remoteaddr.reset(new IPv4Address());
    } break;
    case AF_INET6: {
        remoteaddr.reset(new IPv6Address());
    } break;
    case AF_UNIX: {
        remoteaddr.reset(new UnixAddress());
    } break;
    default: {
        remoteaddr.reset(new UnKnownAddress(m_family));
    }
    }

    socklen_t addrlen = remoteaddr->getAddrLen();
    if (getpeername(m_socket, remoteaddr->getAddr(), &addrlen)) {
        return Address::ptr(new UnKnownAddress(m_family));
    }

    if (m_family == AF_UNIX) {
        std::dynamic_pointer_cast<UnixAddress>(remoteaddr)->setAddrLen(addrlen);
    }
    m_remoteAddress = remoteaddr;
    return m_remoteAddress;
}

int Socket::setOption(int level, int opt_name, const void *opt_value,
                      socklen_t opt_len) {
    if (setsockopt(m_socket, level, opt_name, opt_value, opt_len)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::setOption ( " << m_socket << ", " << level << ", "
            << opt_name << ", " << opt_value << ", " << opt_len
            << ") fail error = " << strerror(errno);
        return false;
    }
    return true;
}

int Socket::getOption(int level, int opt_name, void *opt_value,
                      socklen_t *opt_len) {
    if (getsockopt(m_socket, level, opt_name, opt_value, opt_len)) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::getOption ( " << m_socket << ", " << level << ", "
            << opt_name << ", " << opt_value << ", " << opt_len
            << ") fail error = " << strerror(errno);
        return false;
    }
    return true;
}

bool Socket::init(int socket) {
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(socket);
    if (ctx && ctx->isSockt() && !ctx->isClose()) {
        m_socket = socket;
        m_isConnect = true;
        initSocket();
        getLocalAddress();
        getRemoteAddress();
        return true;
    }
    return false;
}

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
        return false;
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
    getLocalAddress();
    return true;
}

Socket::ptr Socket::accept(Address::ptr address) {
    Socket::ptr socket(new Socket(m_type, m_family, m_protocol));
    int newsock = ::accept(m_socket, nullptr, nullptr);
    if (newsock == -1) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::accept " << address->toString() << " fail";
        return nullptr;
    }
    if (socket->init(newsock)) {
        return socket;
    }
    return nullptr;
}

bool Socket::connect(Address::ptr address, uint64_t timeout_ms) {
    if (!isValid()) {
        newSocket();
        if (LYON_UNLIKELY(!isValid())) {
            return false;
        }
    }

    if (LYON_UNLIKELY(m_family != address->getFamily())) {
        LYON_LOG_ERROR(g_logger)
            << "Socket::connect family not equal. m_family = " << m_family
            << " address family = " << address->getFamily();
        return false;
    }
    if (timeout_ms == (uint64_t)-1) {
        if (::connect(m_socket, address->getAddr(), address->getAddrLen())) {
            LYON_LOG_ERROR(g_logger)
                << "Socket::connect family not equal. m_family = " << m_family
                << " address family = " << address->getFamily();
            return false;
        }
    } else {
        if (::connect_with_timeout(m_socket, address->getAddr(),
                                   address->getAddrLen(), timeout_ms)) {
            LYON_LOG_ERROR(g_logger)
                << "Socket::connect family not equal. m_family = " << m_family
                << " address family = " << address->getFamily();
            return false;
        }
    }
    m_isConnect = true;
    getRemoteAddress();
    getLocalAddress();
    return true;
}

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

ssize_t Socket::recv(void *buffer, size_t length, int flags) {
    if (!isConnect()) {
        return -1;
    }
    return ::recv(m_socket, buffer, length, flags);
}

ssize_t Socket::recv(iovec *buffers, size_t length, int flags) {
    if (!isConnect()) {
        return -1;
    }
    msghdr msg;
    msg.msg_iov = (iovec *)buffers;
    msg.msg_iovlen = length;
    return ::recvmsg(m_socket, &msg, flags);
}

ssize_t Socket::recvFrom(Address::ptr address, void *buffer, size_t length,
                         int flags) {
    if (!isConnect()) {
        return -1;
    }
    socklen_t addrlen = address->getAddrLen();
    return ::recvfrom(m_socket, buffer, length, flags, address->getAddr(),
                      &addrlen);
}

ssize_t Socket::recvFrom(Address::ptr address, iovec *buffers, size_t length,
                         int flags) {
    if (!isConnect()) {
        return -1;
    }
    msghdr msg;
    msg.msg_iov = (iovec *)buffers;
    msg.msg_iovlen = length;
    msg.msg_name = address->getAddr();
    msg.msg_namelen = address->getAddrLen();
    return ::recvmsg(m_socket, &msg, flags);
}

ssize_t Socket::send(const void *buffer, size_t length, int flags) {
    if (!isConnect()) {
        return -1;
    }
    return ::send(m_socket, buffer, length, flags);
}

ssize_t Socket::send(const iovec *buffers, size_t length, int flags) {
    if (!isConnect()) {
        return -1;
    }
    msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = (iovec *)buffers;
    msg.msg_iovlen = length;
    return ::sendmsg(m_socket, &msg, flags);
}

ssize_t Socket::sendTo(Address::ptr address, const void *buffer, size_t length,
                       int flags) {
    if (!isConnect()) {
        return -1;
    }
    return ::sendto(m_socket, buffer, length, flags, address->getAddr(),
                    address->getAddrLen());
}

ssize_t Socket::sendTo(Address::ptr address, const iovec *buffers,
                       size_t length, int flags) {
    if (!isConnect()) {
        return -1;
    }
    msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = (iovec *)buffers;
    msg.msg_iovlen = length;
    msg.msg_name = address->getAddr();
    msg.msg_namelen = address->getAddrLen();
    return ::sendmsg(m_socket, &msg, flags);
}

bool Socket::triggerRead() {
    return IOManager::GetCurrentIOManager()->triggerEvent(m_socket,
                                                          IOManager::READ);
}

bool Socket::triggerWrite() {
    return IOManager::GetCurrentIOManager()->triggerEvent(m_socket,
                                                          IOManager::WRITE);
}

bool Socket::triggerAccept() {
    return IOManager::GetCurrentIOManager()->triggerEvent(m_socket,
                                                          IOManager::READ);
}

bool Socket::triggerAll() {
    return IOManager::GetCurrentIOManager()->triggerAll(m_socket);
}

} // namespace lyon
