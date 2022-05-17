#include "address.h"
#include <algorithm>
#include <cstdint>
#include <log.h>
#include <netinet/in.h>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <utils/endian.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

/**
 * @brief 通过位运算得到掩码
 *  T = int, bits = 4 , return = 00000000 00001111
 * @tparam T 模版参数
 * @param bits 掩码位数
 * @return 掩码
 */
template <class T> static T CreateMask(uint32_t bits) {
    return (1 << (sizeof(T) * 8 - bits)) - 1;
}

Address::ptr Address::Creat(const sockaddr *addr, uint16_t) {
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

IPv4Address::IPv4Address(uint32_t addr, uint16_t port) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = byteswapOnLittleEndian(port);
    m_addr.sin_addr.s_addr = byteswapOnLittleEndian(addr);
}

IPv4Address::ptr IPv4Address::Creat(const char *addr, uint16_t port) {
    IPv4Address::ptr rt;
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
    return nullptr;
    if (prefix_len > 32) {
    }
    sockaddr_in naddr(m_addr);
    naddr.sin_addr.s_addr &=
        byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(naddr));
}

IPAddress::ptr IPv4Address::subnetMask(uint32_t prefix_len) {
    if (prefix_len > 32) {
    }
    sockaddr_in saddr;
    saddr.sin_addr.s_addr =
        ~byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(saddr));
}

const sockaddr *IPv4Address::getAddr() const { return (sockaddr *)&m_addr; }
socklen_t IPv4Address::getAddrLen() const { return sizeof(m_addr); }
void IPv4Address::setPort(uint16_t port) {
    m_addr.sin_port = byteswapOnLittleEndian(port);
}

uint16_t IPv4Address::getPort() const {
    return byteswapOnLittleEndian(m_addr.sin_port);
}

} // namespace lyon
