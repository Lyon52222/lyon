#include "lyon/serialize/serializer.h"
#include <cstdint>
#include <iostream>
#include <lyon/rpc/rpc_method.h>

void test_int() {
    lyon::Serializer ser;
    uint32_t a1 = 24;
    ser << a1;
    ser.setPosition(0);
    uint32_t a2;
    ser >> a2;
    std::cout << a2 << std::endl;
}

void test_string() {
    lyon::Serializer ser;
    std::string str1 = "fdsafdsfs";
    ser << str1;
    ser.setPosition(0);
    std::string str2;
    ser >> str2;
    std::cout << str2 << std::endl;
}

void test_args() {
    lyon::Serializer ser;
    auto &&tuple1 = std::make_tuple(1, 1.3, "abcd");
    ser << tuple1;

    ser.setPosition(0);

    std::tuple<int, double, std::string> tuple2;
    ser >> tuple2;

    std::cout << std::get<0>(tuple2) << std::endl;
    std::cout << std::get<1>(tuple2) << std::endl;
    std::cout << std::get<2>(tuple2) << std::endl;
}

void test_vector() {
    lyon::Serializer ser;
    std::vector<uint32_t> v1 = {1, 2, 3, 4, 5};
    ser << v1;

    ser.setPosition(0);

    std::vector<uint32_t> v2;
    ser >> v2;

    for (size_t i = 0; i < v2.size(); i++) {
        std::cout << v2[i] << ' ';
    }
    std::cout << std::endl;
}

void test_method() {
    auto add = [](int a, int b) -> int { return a + b; };
    lyon::rpc::RPCMethod rpc_add("add", add);

    lyon::Serializer ser;

    ser << 4 << 6;

    ser.setPosition(0);

    rpc_add(ser);

    int ans;

    ser.setPosition(0);
    ser >> ans;

    std::cout << ans << std::endl;
}

int main(int argc, char *argv[]) {
    // test_args();
    test_method();
    return 0;
}
