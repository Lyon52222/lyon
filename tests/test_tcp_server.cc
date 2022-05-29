#include "lyon/iomanager.h"
#include "lyon/log.h"
#include "lyon/tcp_server.h"

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int main(int argc, char *argv[]) {
    lyon::IOManager iom;

    return 0;
}
