#include "lyon/log.h"
#include <memory>
int main(int argc, char *argv[]) {
    std::shared_ptr<lyon::Logger> logger(new lyon::Logger());

    std::shared_ptr<lyon::LogAppender> stdo(new lyon::StdoutLogAppender());
    logger->addAppender(stdo);

    std::shared_ptr<lyon::LogFormatter> formatter(
        new lyon::LogFormatter("log test %m %p %r %t %d %f %l %n %T %F %N"));

    stdo->setFormatter(formatter);

    LYON_LOG_DEBUG(logger) << "this is a test";

    return 0;
}
