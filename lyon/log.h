#ifndef __LYON_LOG_H__
#define __LYON_LOG_H__
#include <list>
#include <memory>
#include <stdint.h>
#include <string>
namespace lyon {

class LogEvent {
  public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

  private:
    const char *m_file = nullptr; //文件名
    int32_t m_line = 0;           //行号
    uint32_t m_threadId = 0;      //线程ID
    uint32_t m_fiberId = 0;       //协程ID
    uint64_t m_time;              //时间戳
    std::string m_content;
};

//日志级别
class LogLevel {
  public:
    enum Level { DEBUG = 1, INFO, WARN, ERROR, FATAL };
};

//日志格式化
class LogFormatter {
  public:
    typedef std::shared_ptr<LogFormatter> ptr;
    std::string format(LogEvent::ptr event);
};

//日志输出地
class LogAppender {
  public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    void log(LogLevel::Level level, LogEvent::ptr event);

  private:
    LogLevel::Level m_level;
};
//输出到控制台的Appender
class StdoutLogAppender : public LogAppender {};

//输出到文件的Appender
class FileLogAppender : public LogAppender {};

//日志输出器
class Logger {
  public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string &name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

  private:
    std::string m_name;
    LogLevel::Level m_level; //满足该级别的日志才会被输出
    std::list<LogAppender::ptr> m_appenders;
};

} // namespace lyon

#endif
