#include "lyon/serialize/serializer.h"
#include <cstdint>
#include <iostream>

void test() {
    lyon::Serializer ser;
    // uint32_t a1 = 24;
    // ser << a1;
    // ser.setPosition(0);
    // uint32_t a2;
    // ser >> a2;
    // std::cout << a2 << std::endl;

    // std::string str1 = "fdsafdsfs";
    // ser << str1;

    // ser.setPosition(0);
    // std::string str2;
    // ser >> str2;
    // std::cout << str2 << std::endl;

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

int main(int argc, char *argv[]) {
    test();
    return 0;
}
