#include "lyon/config.h"
#include "lyon/log.h"
#include <cstdio>
#include <fstream>
#include <iostream>

lyon::ConfigVar<int> a("=number", 10, "test");
int main() {
    std::cout << a.getName();
    return 0;
}
