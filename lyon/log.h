#ifndef __LYON_LOG_H__
#define __LYON_LOG_H__
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <stdint.h>
#include <string>
#include <strstream>
namespace lyon {
//日志级别
class LogLevel {
  public:
    enum Level { UNKNOWN = 0, DEBUG, INFO, WARN, ERROR, FATAL };

    static const char *toString(LogLevel::Level level);
};

class LogEvent {
  public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

    const char *getFile() const { return m_file; };
    int32_t getLine() const { return m_line; };
    uint32_t getThreaId() const { return m_threadId; };
    uint32_t getFiberId() const { return m_fiberId; };
    uint64_t getTime() const { return m_time; };
    std::string getContent() const { return m_content; };
    const LogLevel::Level getLevel() const { return m_level; };

  private:
    const char *m_file = nullptr; //文件名
    int32_t m_line = 0;           //行号
    uint32_t m_threadId = 0;      //线程ID
    uint32_t m_fiberId = 0;       //协程ID
    uint64_t m_time;              //时间戳
    std::string m_content;
    LogLevel::Level m_level;
};

//日志格式化
class LogFormatter {
  public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string &pattern);
    std::string format(LogEvent::ptr event);

    class FormatItem {
      public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem();
        virtual void format(std::ostream &os, LogEvent::ptr event) = 0;
    };

  private:
    std::string m_pattern;
};

//日志输出地
class LogAppender {
  public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr val) { m_formatter = val; };
    LogFormatter::ptr getFormatter() { return m_formatter; };

  protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};
//输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
  public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender {
  public:
    FileLogAppender(const std::string &name);
    typedef std::shared_ptr<FileLogAppender> prt;
    void log(LogLevel::Level level, LogEvent::ptr event) override;

    bool reopen();

  private:
    std::string m_fname;
    std::ofstream m_fstream;
};

//日志输出器
class Logger {
  public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string &name = "root");

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    LogLevel::Level getLevel() const { return m_level; };
    void setLevel(LogLevel::Level val) { m_level = val; };

  private:
    std::string m_name;
    LogLevel::Level m_level; //满足该级别的日志才会被输出
    std::list<LogAppender::ptr> m_appenders;
};

} // namespace lyon

#endif
