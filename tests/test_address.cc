#include "lyon/address.h"
#include <functional>
#include <lyon/hook.h>
#include <lyon/iomanager.h>
#include <lyon/log.h>
#include <memory>
#include <netdb.h>
#include <sys/socket.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test() {
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
}

void test_lookup() {
    std::vector<lyon::Address::ptr> results;
    int rt =
        lyon::Address::LookUp(results, "localhost:8080", AF_INET, SOCK_STREAM);
    if (rt) {
        for (auto &re : results) {
            std::cout << re->toString() << std::endl;
        }
    }
}

void test_getaddrinfo() {
    addrinfo hint, *res, *next;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    // if (getaddrinfo("www.baidu.com", "80", &hint, &res)) {
    //     return;
    // }
    next = res;
    while (next) {
        std::cout << "a" << std::endl;
        next = next->ai_next;
    }

    freeaddrinfo(res);
}

void test_interface() {
    std::multimap<std::string, std::pair<lyon::Address::ptr, uint32_t>> results;
    if (lyon::Address::GetInterfaceAddress(results)) {
        for (auto itr = results.begin(); itr != results.end(); itr++) {
            std::cout << itr->first << " : " << itr->second.first->toString()
                      << "/" << itr->second.second << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom;
    // lyon::set_hook_enable(true);
    // test();
    // test_lookup();
    // test_interface();

    // test_getaddrinfo();
    // iom.addJob(test);
    // iom.addJob(test_lookup);
    iom.addJob(test_getaddrinfo);
    // iom.addJob(test_interface);
    // std::function<void()> cb = test_lookup;
    // cb();
    // cb = nullptr;
    return 0;
}
