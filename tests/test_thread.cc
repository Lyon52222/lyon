#include "lyon/thread.h"
#include <iostream>

void test_a() {
    for (int i = 0; i < 100; i++) {
        std::cout << 'a' << std ::endl;
    }
}

void test_b() {
    for (int i = 0; i < 100; i++) {
        std::cout << 'b' << std ::endl;
    }
}

int main(int argc, char *argv[]) {
    lyon::Thread thread_a(test_a, "testa");
    lyon::Thread thread_b(test_b, "testb");
    thread_a.join();
    thread_b.join();
    return 0;
}
