#include "lyon/log.h"
#include <memory>
int main(int argc, char *argv[]) {

    lyon::Logger::ptr logger = LYON_LOG_GET_ROOT();
    std::shared_ptr<lyon::LogAppender> stdo(new lyon::StdoutLogAppender());
    logger->addAppender(stdo);

    LYON_LOG_DEBUG(logger) << "this is a test";

    return 0;
}
