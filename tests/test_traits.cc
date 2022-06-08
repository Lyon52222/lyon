#include "lyon/traits.h"
#include <functional>
#include <iostream>

template <typename T> void PrintType() {
    std::cout << typeid(T).name() << std::endl;
}

void test_function_traits() {
    std::function<int(int)> f = [](int a) { return a; };

    // 打印函数类型
    PrintType<
        lyon::function_traits<std::function<int(int, int)>>::function_type>();

    //打印函数的参数个数
    std::cout << lyon::function_traits<std::function<int(int, float)>>::arity
              << std::endl;

    // 打印函数的第一个参数类型
    PrintType<
        lyon::function_traits<std::function<int(int, float)>>::args<1>::type>();

    //打印函数返回值类型
    PrintType<lyon::function_traits<decltype(f)>::return_type>();

    //打印函数指针类型
    PrintType<lyon::function_traits<decltype(f)>::pointer>();
}

int main(int argc, char *argv[]) {
    test_function_traits();
    return 0;
}
