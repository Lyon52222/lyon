#ifndef __LYON_SOCKET_H__
#define __LYON_SOCKET_H__
#include <address.h>
#include <cstddef>
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

    bool isValid() const;

    void newSocket();
    void initSocket();

    virtual bool bind(Address::ptr address);
    virtual bool listen(int backlog);
    int accept(Address::ptr address);
    int connect(Address::ptr address);

    bool close();

    ssize_t read(void *buf, size_t nbyte);

    ssize_t write(void *buf, size_t nbyte);

    ssize_t recvFrom(Address::ptr address, void *buffer, size_t length,
                     int flags = 0);

    ssize_t sendTo(Address::ptr address, void *message, size_t length,
                   int flags = 0);

    Socket(int type, int family, int protocol = 0);
    ~Socket();

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
