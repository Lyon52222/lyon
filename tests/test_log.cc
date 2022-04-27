#include "lyon/log.h"
#include <memory>
int main(int argc, char *argv[]) {
    std::shared_ptr<lyon::Logger> logger(new lyon::Logger());

    std::shared_ptr<lyon::LogAppender> stdo(new lyon::StdoutLogAppender());
    logger->addAppender(stdo);

    std::shared_ptr<lyon::LogFormatter> formatter(
        new lyon::LogFormatter("log test %m %p %r %t %d %f %l %n %T %F %N"));

    stdo->setFormatter(formatter);
    std::string threadName = "abc";
    std::string content = "this is a test";
    std::shared_ptr<lyon::LogEvent> event(
        new lyon::LogEvent(__FILE__, __LINE__, 0, threadName, 55555, 44444,
                           4343, content, lyon::LogLevel::FATAL));

    logger->warn(event);
    return 0;
}
