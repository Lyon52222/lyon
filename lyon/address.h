#ifndef __LYON__ADDRESS_H__
#define __LYON__ADDRESS_H__
#include <arpa/inet.h>
#include <cstdint>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <vector>
namespace lyon {

class IPAddress;

/**
 * @brief 网络地址的抽象基类
 */
class Address {
public:
    typedef std::shared_ptr<Address> ptr;

    /**
     * @brief 通过sockaddr创建Address
     *
     * @param addr sockaddr
     * @param port 端口号
     */
    static Address::ptr Create(const sockaddr *addr, socklen_t addrlen);

    /**
     * @brief 通过host地址返回满足条件的所有Address
     *
     * @param results 满足条件的所有Address
     * @param host host地址
     * 域名，服务器名等等。例如www.baidu.com[112.14.123.1:8080]
     * @param AF_INET 地址协议族，AF_INET，AF_INET6,AF_UNIX
     * @param type 数据类型，SOCK_DGRAM, SOCK_STREAM等
     * @param protocol IP的上层协议族，IPPROTO_TCP, IPPROTO_UDP等
     * @return 查询是否成功
     */
    static bool LookUp(std::vector<Address::ptr> &results,
                       const std::string &host, int family = AF_INET,
                       int type = 0, int protocol = 0);

    static Address::ptr LookUpAny(const std::string &host, int family = AF_INET,
                                  int type = 0, int protocol = 0);

    static std::shared_ptr<IPAddress>
    LookUpAnyIpAddress(const std::string &host, int family = AF_INET,
                       int type = 0, int protocol = 0);

    static bool GetInterfaceAddress(
        std::multimap<std::string, std::pair<Address::ptr, uint32_t>>
            &if_address,
        int family = AF_INET);

    static bool GetInferfaceAddress(
        std::string &if_name,
        std::vector<std::pair<Address::ptr, uint32_t>> &if_address,
        int family = AF_INET);
    /**
     * @brief 获取该地址的协议族
     *
     * @return 协议族
     */
    int getFamily();

    /**
     * @brief 将地址转换为string
     *
     */
    std::string toString() const;

    /**
     * @brief 获取地址
     *
     */
    virtual const sockaddr *getAddr() const = 0;

    virtual sockaddr *getAddr() = 0;
    /**
     * @brief 获取地址长度
     *
     * @return 地址长度
     */
    virtual socklen_t getAddrLen() const = 0;
    /**
     * @brief 将地址转化为字符串并插入流中
     *
     * @param os 输出流
     */
    virtual std::ostream &insert(std::ostream &os) const = 0;

    bool operator<(const Address &rhs) const;
    bool operator==(const Address &rhs) const;
    bool operator!=(const Address &rhs) const;

    virtual ~Address() {}
};

class IPAddress : public Address {
public:
    typedef std::shared_ptr<IPAddress> ptr;

    static IPAddress::ptr Create(const char *addr, uint16_t port = 0);

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
    IPv4Address(const sockaddr_in &addr);
    IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

    static IPv4Address::ptr Create(const char *addr, uint16_t port = 0);

    IPAddress::ptr broadCastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networkAddress(uint32_t prefix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;
    sockaddr *getAddr() override;
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
    IPv6Address(const uint8_t addr[16], uint16_t port = 0);
    static IPv6Address::ptr Create(const char *addr, uint16_t port = 0);

    IPAddress::ptr broadCastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networkAddress(uint32_t prefix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;
    sockaddr *getAddr() override;
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
    typedef std::shared_ptr<UnixAddress> ptr;

    UnixAddress();
    UnixAddress(const std::string &path);
    sockaddr *getAddr() override;
    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    void setAddrLen(socklen_t addr_len) { m_len = addr_len; }
    std::ostream &insert(std::ostream &os) const override;
    std::string getPath() const;

private:
    sockaddr_un m_addr;
    socklen_t m_len;
};

class UnKnownAddress : public Address {
public:
    typedef std::shared_ptr<UnixAddress> ptr;
    UnKnownAddress(int family);
    UnKnownAddress(const sockaddr &addr);

    sockaddr *getAddr() override;
    const sockaddr *getAddr() const override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;

private:
    sockaddr m_addr;
};

} // namespace lyon

#endif
