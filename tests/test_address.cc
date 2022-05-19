#include "lyon/address.h"
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int main(int argc, char *argv[]) {
    lyon::IPv4Address::ptr ipv4 =
        lyon::IPv4Address::Create("192.168.2.1", 8080);
    LYON_LOG_INFO(g_logger) << ipv4->toString();
    LYON_LOG_INFO(g_logger) << ipv4->broadCastAddress(24)->toString();
    LYON_LOG_INFO(g_logger) << ipv4->networkAddress(24)->toString();
    LYON_LOG_INFO(g_logger) << ipv4->subnetMask(24)->toString();

    lyon::IPv6Address::ptr ipv6_0 = lyon::IPv6Address::Create(
        "ABCD:EF01:0000:0000:0000:EF01:2345:6789", 8081);
    LYON_LOG_INFO(g_logger) << ipv6_0->toString();
    lyon::IPv6Address::ptr ipv6_1 = lyon::IPv6Address::Create(
        "0000:0000:0000:0000:0000:0000:0000:0000", 8081);
    LYON_LOG_INFO(g_logger) << ipv6_1->toString();
    lyon::IPv6Address::ptr ipv6_2 = lyon::IPv6Address::Create(
        "ABCD:EF01:2345:6789:ABCD:EF01:2345:6789", 8081);
    LYON_LOG_INFO(g_logger) << ipv6_2->toString();

    LYON_LOG_INFO(g_logger) << ipv6_2->broadCastAddress(56)->toString();
    LYON_LOG_INFO(g_logger) << ipv6_2->networkAddress(56)->toString();
    LYON_LOG_INFO(g_logger) << ipv6_2->subnetMask(56)->toString();

    return 0;
}
