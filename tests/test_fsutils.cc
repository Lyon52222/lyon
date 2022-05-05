#include "lyon/utils/file_system_util.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    std::ofstream ofs;

    lyon::FSUtil::OpenForWrite(ofs, "/apps/logs/a.txt", std::ios::app);
    std::cout << ofs.is_open() << std::endl;
    ofs << "test\n" << std::endl;
    return 0;
}
