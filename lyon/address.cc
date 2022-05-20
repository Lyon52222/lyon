#include "address.h"
#include "log.h"
#include "utils/endian.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <ifaddrs.h>
#include <map>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <utility>
#include <vector>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

/**
 * @brief 通过位运算得到掩码
 *  T = int, bits = 4 , return = 00001111 11111111
 * @tparam T 模版参数
 * @param bits 掩码位数
 * @return 掩码
 */
template <class T> static T CreateMask(uint32_t bits) {
    return (1 << (sizeof(T) * 8 - bits)) - 1;
}

template <class T> static uint32_t CountMaskLen(T mask) {
    uint32_t len = 0;
    while (mask) {
        len++;
        mask &= mask - 1;
    }
    return len;
}

Address::ptr Address::Create(const sockaddr *addr, socklen_t addrlen) {
    if (!addr) {
        return nullptr;
    }
    Address::ptr result;
    if (addr->sa_family == AF_INET) {
        result.reset(new IPv4Address(*(const sockaddr_in *)addr));
    } else if (addr->sa_family == AF_INET6) {
        result.reset(new IPv6Address(*(const sockaddr_in6 *)addr));
    } else {
        result.reset(new UnKnownAddress(*addr));
    }
    return result;
}

bool Address::LookUp(std::vector<Address::ptr> &results,
                     const std::string &host, int family, int type,
                     int protocol) {
    addrinfo hint, *res, *next;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = family;
    hint.ai_socktype = type;
    hint.ai_protocol = protocol;

    //先查看有没有附属内容：www.baidu.com[183.134.11.1:80]
    std::string node;
    const char *service = nullptr;
    if (!host.empty() && host[0] == '[') {
        const char *endipv6 =
            (char *)memchr(host.c_str() + 1, ']', host.size() - 1);
        if (endipv6) {
            if (*(endipv6 + 1) == ':') {
                service = endipv6 + 2;
            }
            node = host.substr(1, endipv6 - host.c_str() - 1);
        }
    }
    //没有附属内容：193.134.14.1:80 / www.baidu.com:80
    if (node.empty()) {
        service = (char *)memchr(host.c_str(), ':', host.size());
        if (service) {
            if (!memchr(service + 1, ':',
                        host.c_str() + host.size() - service - 1)) {
                node = host.substr(0, service - host.c_str());
                service++;
            }
        }
    }

    //没有端口号：www.baidu.com
    if (node.empty()) {
        node = host;
    }
    int rt = getaddrinfo(node.c_str(), service, &hint, &res);
    if (rt) {
        LYON_LOG_ERROR(g_logger)
            << "Address LookUp host : " << host << " family : " << family
            << " type : " << type << " protocol : " << protocol << " fail";
        return false;
    }
    next = res;
    while (next) {
        results.push_back(Create(next->ai_addr, next->ai_addrlen));
        next = next->ai_next;
    }

    freeaddrinfo(res);
    return !results.empty();
}

Address::ptr Address::LookUpAny(const std::string &host, int family, int type,
                                int protocol) {
    std::vector<Address::ptr> results;
    if (LookUp(results, host, family, type, protocol)) {
        return results[0];
    }
    return nullptr;
}

std::shared_ptr<IPAddress> Address::LookUpAnyIpAddress(const std::string &host,
                                                       int family, int type,
                                                       int protocol) {
    std::vector<Address::ptr> results;
    if (LookUp(results, host, family, type, protocol)) {
        for (auto &re : results) {
            IPAddress::ptr ipa = std::dynamic_pointer_cast<IPAddress>(re);
            if (ipa) {
                return ipa;
            }
        }
        return nullptr;
    }
    return nullptr;
}

bool Address::GetInterfaceAddress(
    std::multimap<std::string, std::pair<Address::ptr, uint32_t>> &if_address,
    int family) {
    ifaddrs *next, *results;
    int rt = getifaddrs(&results);
    if (rt) {
        LYON_LOG_ERROR(g_logger) << "Address::GetInferfaceAddress fail";
        freeifaddrs(results);
        return false;
    }
    Address::ptr addr;
    uint32_t prefix_len;
    try {
        for (next = results; next; next = next->ifa_next) {
            addr.reset();
            prefix_len = ~0u;
            switch (next->ifa_addr->sa_family) {
            case AF_INET: {
                addr = Create(next->ifa_addr, sizeof(sockaddr_in));
                // sockaddr 和 sockaddr_in之间是可以相互转换的
                uint32_t netmask =
                    ((sockaddr_in *)next->ifa_netmask)->sin_addr.s_addr;
                prefix_len = CountMaskLen(netmask);
            } break;
            case AF_INET6: {
                addr = Create(next->ifa_addr, sizeof(sockaddr_in6));
                in6_addr &netmask =
                    ((sockaddr_in6 *)next->ifa_netmask)->sin6_addr;
                prefix_len = 0;
                for (size_t i = 0; i < 16; i++) {
                    prefix_len += CountMaskLen(netmask.s6_addr[i]);
                }

            } break;
            default:
                break;
            }

            if (addr) {
                if_address.insert(std::make_pair(
                    next->ifa_name, std::make_pair(addr, prefix_len)));
            }
        }
    } catch (...) {
        freeifaddrs(results);
        return false;
    }
    freeifaddrs(results);
    return !if_address.empty();
}

bool Address::GetInferfaceAddress(
    std::string &if_name,
    std::vector<std::pair<Address::ptr, uint32_t>> &if_address, int family) {
    if (if_name.empty() || if_name == "*") {
        if (family == AF_INET || family == AF_UNSPEC) {
            if_address.push_back(
                std::make_pair(Address::ptr(new IPv4Address()), 0u));
        }
        if (family == AF_INET6 || family == AF_UNSPEC) {
            if_address.push_back(
                std::make_pair(Address::ptr(new IPv6Address()), 0u));
        }
        return true;
    }

    std::multimap<std::string, std::pair<Address::ptr, uint32_t>> results;
    if (GetInterfaceAddress(results, family)) {
        auto itr = results.equal_range(if_name);
        for (auto i = itr.first; i != itr.second; i++) {
            if_address.push_back(i->second);
        }
    }

    return !if_address.empty();
}
int Address::getFamily() { return getAddr()->sa_family; }

std::string Address::toString() const {
    std::stringstream ss;
    insert(ss);
    return ss.str();
}

bool Address::operator<(const Address &rhs) const {
    socklen_t minlen = std::min(getAddrLen(), rhs.getAddrLen());
    int result = memcmp(getAddr(), rhs.getAddr(), minlen);
    if (result < 0) {
        return true;
    } else if (result > 0) {
        return false;
    } else if (getAddrLen() < rhs.getAddrLen()) {
        return true;
    }
    return false;
}

bool Address::operator==(const Address &rhs) const {
    return getAddrLen() == rhs.getAddrLen() &&
           memcmp(getAddr(), rhs.getAddr(), getAddrLen()) == 0;
}

bool Address::operator!=(const Address &rhs) const { return !(*this == rhs); }

IPAddress::ptr IPAddress::Create(const char *addr, uint16_t port) {
    addrinfo hint, *res;
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;
    //获取地址信息
    int rt = getaddrinfo(addr, nullptr, &hint, &res);
    if (rt) {
        LYON_LOG_ERROR(g_logger) << "IPAddress create : " << addr << "fail";
        return nullptr;
    }
    try {
        IPAddress::ptr result = std::dynamic_pointer_cast<IPAddress>(
            Address::Create(res->ai_addr, res->ai_addrlen));
        if (result) {
            result->setPort(port);
        }
        freeaddrinfo(res);
        return result;
    } catch (...) {
        freeaddrinfo(res);
        return nullptr;
    }
    return nullptr;
}

IPv4Address::IPv4Address(const sockaddr_in &addr) { m_addr = addr; }

IPv4Address::IPv4Address(uint32_t addr, uint16_t port) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = byteswapOnLittleEndian(port);
    m_addr.sin_addr.s_addr = byteswapOnLittleEndian(addr);
}

IPv4Address::ptr IPv4Address::Create(const char *addr, uint16_t port) {
    IPv4Address::ptr rt(new IPv4Address());
    rt->m_addr.sin_port = byteswapOnLittleEndian(port);
    int n = inet_pton(AF_INET, addr, &rt->m_addr.sin_addr);
    if (n != 1) {
        LYON_LOG_ERROR(g_logger)
            << "Convert " << addr << "to IPv4 addr fail. rt = " << rt;
        return nullptr;
    }
    return rt;
}

IPAddress::ptr IPv4Address::broadCastAddress(uint32_t prefix_len) {
    if (prefix_len > 32) {
        return nullptr;
    }
    sockaddr_in baddr(m_addr);
    baddr.sin_addr.s_addr |=
        byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(baddr));
}

IPAddress::ptr IPv4Address::networkAddress(uint32_t prefix_len) {
    if (prefix_len > 32) {
        return nullptr;
    }
    sockaddr_in naddr(m_addr);
    naddr.sin_addr.s_addr &=
        ~byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(naddr));
}

IPAddress::ptr IPv4Address::subnetMask(uint32_t prefix_len) {
    if (prefix_len > 32) {
        return nullptr;
    }
    sockaddr_in saddr;
    saddr.sin_port = 0;
    saddr.sin_addr.s_addr =
        ~byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(saddr));
}

const sockaddr *IPv4Address::getAddr() const { return (sockaddr *)&m_addr; }

socklen_t IPv4Address::getAddrLen() const { return sizeof(m_addr); }

std::ostream &IPv4Address::insert(std::ostream &os) const {
    uint32_t addr = byteswapOnLittleEndian(m_addr.sin_addr.s_addr);
    os << ((addr >> 24) & 0xff) << '.' << ((addr >> 16) & 0xff) << '.'
       << ((addr >> 8) & 0xff) << '.' << ((addr)&0xff);
    os << ':' << byteswapOnLittleEndian(m_addr.sin_port);
    return os;
}

void IPv4Address::setPort(uint16_t port) {
    m_addr.sin_port = byteswapOnLittleEndian(port);
}

uint16_t IPv4Address::getPort() const {
    return byteswapOnLittleEndian(m_addr.sin_port);
}

IPv6Address::IPv6Address() {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin6_family = AF_INET6;
}

IPv6Address::IPv6Address(const sockaddr_in6 &addr) { m_addr = addr; }

IPv6Address::IPv6Address(const uint8_t addr[16], uint16_t port) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin6_family = AF_INET6;
    m_addr.sin6_port = byteswapOnLittleEndian(port);
    memcpy(&m_addr.sin6_addr.s6_addr, addr, 16);
}

IPv6Address::ptr IPv6Address::Create(const char *addr, uint16_t port) {
    IPv6Address::ptr rt(new IPv6Address());
    rt->m_addr.sin6_port = byteswapOnLittleEndian(port);
    int n = inet_pton(AF_INET6, addr, &rt->m_addr.sin6_addr);
    if (n != 1) {
        LYON_LOG_ERROR(g_logger)
            << "Convert " << addr << "to IPv6 addr fail. rt = " << rt;
        return nullptr;
    }
    return rt;
}

IPAddress::ptr IPv6Address::broadCastAddress(uint32_t prefix_len) {
    if (prefix_len > 8 * 16) {
        return nullptr;
    }
    sockaddr_in6 baddr(m_addr);
    for (int i = prefix_len / 8 + 1; i < 16; i++) {
        baddr.sin6_addr.s6_addr[i] = 0xff;
    }
    baddr.sin6_addr.s6_addr[prefix_len / 8] |=
        CreateMask<uint8_t>(prefix_len % 8);
    return IPv6Address::ptr(new IPv6Address(baddr));
}

IPAddress::ptr IPv6Address::networkAddress(uint32_t prefix_len) {
    if (prefix_len > 8 * 16) {
        return nullptr;
    }
    sockaddr_in6 naddr(m_addr);
    for (size_t i = prefix_len / 8 + 1; i < 16; i++) {
        naddr.sin6_addr.s6_addr[i] = 0x00;
    }
    naddr.sin6_addr.s6_addr[prefix_len / 8] &=
        ~CreateMask<uint8_t>(prefix_len % 8);
    return IPv6Address::ptr(new IPv6Address(naddr));
}

IPAddress::ptr IPv6Address::subnetMask(uint32_t prefix_len) {
    if (prefix_len > 8 * 16) {
        return nullptr;
    }
    sockaddr_in6 saddr;
    memset(&saddr, 0, sizeof(saddr));
    for (size_t i = 0; i < prefix_len / 8; i++) {
        saddr.sin6_addr.s6_addr[i] = 0xff;
    }
    saddr.sin6_addr.s6_addr[prefix_len / 8] =
        ~CreateMask<uint8_t>(prefix_len % 8);
    return IPv6Address::ptr(new IPv6Address(saddr));
}

const sockaddr *IPv6Address::getAddr() const { return (sockaddr *)&m_addr; }

socklen_t IPv6Address::getAddrLen() const { return sizeof(m_addr); }

std::ostream &IPv6Address::insert(std::ostream &os) const {
    os << '[';
    uint16_t *addr = (uint16_t *)m_addr.sin6_addr.s6_addr;
    bool use_zero = false;
    for (int i = 0; i < 8; i++) {
        if (addr[i] == 0 && !use_zero) {
            continue;
        }
        if (i && addr[i - 1] == 0 && !use_zero) {
            os << ':';
            use_zero = true;
        }
        if (i)
            os << ':';
        os << std::hex << (int)byteswapOnLittleEndian(addr[i]) << std::dec;
    }
    if (!use_zero && addr[7] == 0) {
        os << "::";
    }
    os << "]:" << byteswapOnLittleEndian(m_addr.sin6_port);
    return os;
}

void IPv6Address::setPort(uint16_t port) {
    m_addr.sin6_port = byteswapOnLittleEndian(port);
}

uint16_t IPv6Address::getPort() const {
    return byteswapOnLittleEndian(m_addr.sin6_port);
}

static const size_t MAX_PATH_LEN = sizeof(((sockaddr_un *)0)->sun_path) - 1;

UnixAddress::UnixAddress() {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sun_family = AF_UNIX;
    //获取结构体成员变量的相对偏移量
    m_len = offsetof(sockaddr_un, sun_path) + MAX_PATH_LEN;
}

// INFO:搞不懂。。。
UnixAddress::UnixAddress(const std::string &path) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sun_family = AF_UNIX;
    m_len = path.size() + 1;
    if (!path.empty() && path[0] == '\0') {
        m_len--;
    }
    if (m_len > sizeof(m_addr.sun_path)) {
        throw std::logic_error("path is too long");
    }
    memcpy(m_addr.sun_path, path.c_str(), m_len);
    m_len += offsetof(sockaddr_un, sun_path);
}
const sockaddr *UnixAddress::getAddr() const { return (sockaddr *)&m_addr; }

socklen_t UnixAddress::getAddrLen() const { return m_len; }

std::ostream &UnixAddress::insert(std::ostream &os) const {
    if (m_len > offsetof(sockaddr_un, sun_path) && m_addr.sun_path[0] == '\0') {
        return os << "\\0"
                  << std::string(m_addr.sun_path + 1,
                                 m_len - offsetof(sockaddr_un, sun_path) - 1);
    }
    return os << m_addr.sun_path;
}

std::string UnixAddress::getPath() const {
    std::stringstream ss;
    if (m_len > offsetof(sockaddr_un, sun_path) && m_addr.sun_path[0] == '\0') {
        ss << "\\0"
           << std::string(m_addr.sun_path + 1,
                          m_len - offsetof(sockaddr_un, sun_path) - 1);
    }
    ss << m_addr.sun_path;
    return ss.str();
}

UnKnownAddress::UnKnownAddress(int family) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sa_family = family;
}

UnKnownAddress::UnKnownAddress(const sockaddr &addr) { m_addr = addr; }

const sockaddr *UnKnownAddress::getAddr() const { return (sockaddr *)&m_addr; }
socklen_t UnKnownAddress::getAddrLen() const { return sizeof(m_addr); }
std::ostream &UnKnownAddress::insert(std::ostream &os) const {
    os << "[UnKnownAddress] family = " << m_addr.sa_family;
    return os;
}
} // namespace lyon
