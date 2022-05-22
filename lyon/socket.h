#ifndef __LYON_SOCKET_H__
#define __LYON_SOCKET_H__
#include "address.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
namespace lyon {

class Socket : public std::enable_shared_from_this<Socket> {
public:
    typedef std::shared_ptr<Socket> ptr;
    typedef std::weak_ptr<Socket> weak_ptr;

    enum Type { TCP = SOCK_STREAM, UDP = SOCK_DGRAM };
    enum Family { IPv4 = AF_INET, IPv6 = AF_INET6, Unix = AF_UNIX };

    static Socket::ptr CreateTCP(Address::ptr address);
    static Socket::ptr CreateUDP(Address::ptr address);

    static Socket::ptr CreateTCPSocket();
    static Socket::ptr CreateTCPSocket6();
    static Socket::ptr CreateTCPUnixSocket();

    static Socket::ptr CreateUDPSocket();
    static Socket::ptr CreateUDPSocket6();
    static Socket::ptr CreateTCPUnixSocket6();

    uint64_t getRecvTimeout() const;
    void setRecvTimeout(uint64_t timeout);

    uint64_t getSendTimeout() const;
    void setSendTimeout(uint64_t timeout);

    Address::ptr getLocalAddress();
    Address::ptr getRemoteAddress();

    int setOption(int level, int opt_name, const void *opt_value,
                  socklen_t opt_len);

    template <class T>
    int setOption(int level, int opt_name, const T &opt_value) {
        return setOption(level, opt_name, &opt_value, sizeof(T));
    }

    int getOption(int level, int opt_name, void *opt_value, socklen_t *opt_len);

    template <class T> int getOption(int level, int opt_name, T &opt_value) {
        int opt_len = sizeof(T);
        return getOption(level, opt_name, &opt_value, &opt_len);
    }

    bool isValid() const { return m_socket != -1; };
    bool isConnect() const { return m_isConnect; }

    virtual bool init(int socket);

    virtual bool bind(Address::ptr address);
    virtual bool listen(int backlog);
    virtual Socket::ptr accept(Address::ptr address);
    virtual bool connect(Address::ptr address, uint64_t timeout_ms = -1);

    bool close();

    ssize_t recv(void *buffer, size_t length, int flags = 0);

    ssize_t recv(iovec *buffers, size_t length, int flags = 0);

    ssize_t recvFrom(Address::ptr address, void *buffer, size_t length,
                     int flags = 0);

    ssize_t recvFrom(Address::ptr address, iovec *buffers, size_t length,
                     int flags = 0);

    ssize_t send(const void *buffer, size_t length, int flags = 0);

    ssize_t send(const iovec *buffers, size_t length, int flags = 0);

    ssize_t sendTo(Address::ptr address, const void *buffer, size_t length,
                   int flags = 0);

    ssize_t sendTo(Address::ptr address, const iovec *buffers, size_t length,
                   int flags = 0);

    Socket(int type, int family, int protocol = 0);
    ~Socket();

    int getSocket() const { return m_socket; }
    int getType() const { return m_type; }
    int getFamily() const { return m_family; }

    bool triggerRead();
    bool triggerWrite();
    bool triggerAccept();
    bool triggerAll();

private:
    void newSocket();
    void initSocket();

private:
    int m_socket;
    Type m_type;
    Family m_family;
    int m_protocol;
    bool m_isConnect;
    Address::ptr m_localAddress;
    Address::ptr m_remoteAddress;
};
} // namespace lyon
#endif
