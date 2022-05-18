#ifndef __LYON__ADDRESS_H__
#define __LYON__ADDRESS_H__
#include <arpa/inet.h>
#include <cstdint>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
namespace lyon {

class Address {
public:
    typedef std::shared_ptr<Address> ptr;

    static Address::ptr Creat(const sockaddr *addr, uint16_t port = 0);

    int getFamily();
    std::string toString() const;
    virtual const sockaddr *getAddr() const = 0;
    virtual socklen_t getAddrLen() const = 0;
    virtual std::ostream &insert(std::ostream &os) const = 0;

    bool operator<(const Address &rhs) const;
    bool operator==(const Address &rhs) const;
    bool operator!=(const Address &rhs) const;
};

class IPAddress : public Address {
public:
    typedef std::shared_ptr<IPAddress> ptr;

    static IPAddress::ptr Creat(const char *addr, uint16_t port = 0);

    virtual IPAddress::ptr broadCastAddress(uint32_t prefix_len) = 0;
    virtual IPAddress::ptr networkAddress(uint32_t prefix_len) = 0;
    virtual IPAddress::ptr subnetMask(uint32_t prefix_len) = 0;

    virtual void setPort(uint16_t port) = 0;
    virtual uint16_t getPort() const = 0;

private:
    socklen_t m_len;
};

class IPv4Address : public IPAddress {

public:
    typedef std::shared_ptr<IPv4Address> ptr;
    IPv4Address();
    IPv4Address(const sockaddr_in &addr);
    IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

    static IPv4Address::ptr Creat(const char *addr, uint16_t port = 0);

    IPAddress::ptr broadCastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networkAddress(uint32_t prefix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;
    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;
    void setPort(uint16_t port) override;
    uint16_t getPort() const override;

private:
    sockaddr_in m_addr;
};

class IPv6Address : public IPAddress {
public:
    typedef std::shared_ptr<IPv6Address> ptr;
    IPv6Address();
    IPv6Address(const sockaddr_in6 &addr);
    IPv6Address(const uint8_t addr[16], uint16_t port);
    static IPv6Address::ptr Creat(const char *addr, uint16_t port = 0);

    IPAddress::ptr broadCastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networkAddress(uint32_t prefix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;
    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;
    void setPort(uint16_t port) override;
    uint16_t getPort() const override;

private:
    sockaddr_in6 m_addr;
};

class UnixAddress : public Address {
public:
    std::shared_ptr<UnixAddress> ptr;

    UnixAddress();
    UnixAddress(const std::string &path);
    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;
    std::string getPath() const;

private:
    sockaddr_un m_addr;
    socklen_t m_len;
};

class UnKnownAddress : public Address {
public:
    typedef std::shared_ptr<UnixAddress> ptr;
    UnKnownAddress(const sockaddr &addr);

    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;
};

} // namespace lyon

#endif
