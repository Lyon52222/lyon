#include "lyon/macro.h"
#include "lyon/util.h"
#include <iostream>
#include <lyon/log.h>

lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

int main(int argc, char *argv[]) {
    // LYON_LOG_INFO(g_logger) << lyon::BackTraceToString(3, 0, "test");
    LYON_ASSERT(0);
    return 0;
}
