#include "log.h"
int main(int argc, char *argv[]) {

    lyon::LogFormatter formatter("%{ac %d");
    formatter.init();
    return 0;
}
