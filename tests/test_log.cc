#include "lyon/log.h"
#include <memory>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int main(int argc, char *argv[]) {

    LYON_LOG_DEBUG(g_logger) << "this is a test";

    return 0;
}
