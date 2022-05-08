#ifndef __LYON_LOG_H__
#define __LYON_LOG_H__
#include "mutex.h"
#include "singleton.h"
#include "thread.h"
#include "util.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

#define LYON_LOG_LEVEL(logger, level)                                          \
    if (logger->getLevel() <= level)                                           \
    lyon::LogEventWrap(                                                        \
        lyon::LogEvent::ptr(new lyon::LogEvent(                                \
            logger, __FILE__, __LINE__, lyon::GetCurrentThreadId(),            \
            lyon::Thread::GetName(), lyon::GetCurrentFiberId(), time(0), 0,    \
            level)))                                                           \
        .getSS()

#define LYON_LOG_DEBUG(logger) LYON_LOG_LEVEL(logger, lyon::LogLevel::DEBUG)
#define LYON_LOG_INFO(logger) LYON_LOG_LEVEL(logger, lyon::LogLevel::INFO)
#define LYON_LOG_WARN(logger) LYON_LOG_LEVEL(logger, lyon::LogLevel::WARN)
#define LYON_LOG_ERROR(logger) LYON_LOG_LEVEL(logger, lyon::LogLevel::ERROR)
#define LYON_LOG_FATAL(logger) LYON_LOG_LEVEL(logger, lyon::LogLevel::FATAL)

#define LYON_LOG_GET_ROOT() lyon::LoggerMgr::getInstance()->getRoot()
#define LYON_LOG_GET_LOGGER(name)                                              \
    lyon::LoggerMgr::getInstance()->getLogger(name)

namespace lyon {

class Logger;
//日志级别
class LogLevel {
public:
    enum Level { UNKNOWN = 0, DEBUG, INFO, WARN, ERROR, FATAL };

    static const char *toString(LogLevel::Level level);
    static LogLevel::Level fromString(const std::string &level);
};

/**
 * @brief Log事件类，这个类主要对一个log事件进行封装。
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, const char *file, int32_t line,
             uint64_t threadId, const std::string &threadName, uint32_t fiberId,
             uint64_t time, uint32_t elapse, LogLevel::Level level);

    const char *getFile() const { return m_file; };
    int32_t getLine() const { return m_line; };
    uint64_t getThreadId() const { return m_threadId; };
    std::string getThreadName() const { return m_threadName; };
    uint32_t getFiberId() const { return m_fiberId; };
    uint64_t getTime() const { return m_time; };
    uint32_t getElapse() const { return m_elapse; };
    std::string getContent() const { return m_ss.str(); };
    std::stringstream &getSS() { return m_ss; };
    std::shared_ptr<Logger> getLogger() const { return m_logger; };
    const LogLevel::Level getLevel() const { return m_level; };

private:
    std::shared_ptr<Logger> m_logger;
    const char *m_file = nullptr; //文件名
    int32_t m_line = 0;           //行号
    uint64_t m_threadId = 0;      //线程ID
    std::string m_threadName;     //线程名称
    uint32_t m_fiberId = 0;       //协程ID
    uint64_t m_time = 0;          //时间戳
    uint32_t m_elapse = 0;
    // std::string m_content;
    /**
     * @m_ss 这里使用stringstream 来代替string这样可以将信息保存到流中。
     */
    std::stringstream m_ss;

    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    LogEventWrap(LogEvent::ptr event) : m_event(event){};
    /**
     * @brief
     * 在析构时调用event对应的logger对暂存在stringstream中的content进行统一的输出
     *
     */
    ~LogEventWrap();

    std::stringstream &getSS() { return m_event->getSS(); };

private:
    LogEvent::ptr m_event;
};

//日志格式化
/**
 * %m 消息
 * %p 日志等级
 * %r 累计毫秒数
 * %t 当前线程ID
 * %d 时间和日期
 * %f 文件名
 * %l 源码行数
 * %n 换行符
 * %T 制表符
 * %F 协程ID
 * %N 线程名
 * %c 日志名称
 * 默认formatter'pattern = "%d{%Y-%m-%d
 %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n";

 */
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    LogFormatter(const std::string &pattern);
    void setPattern(std::string pattern);
    /**
     * @brief 将m_pattern解析为对应的FormatItem并保存在m_items中
     *
     */
    void parsePattern();
    /**
     * @brief 以pattern格式直接返回一个log描述
     *
     * @param logger 日志器
     * @param level 日志等级
     * @param event 日志事件
     */
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                       LogEvent::ptr event);
    /**
     * @brief 以pattern格式格式化事件，并直接输出到流
     *
     * @param logger 日志器
     * @param level 日志等级
     * @param event 日志事件
     */
    std::ostream &format(std::ostream &os, std::shared_ptr<Logger> logger,
                         LogLevel::Level level, LogEvent::ptr event);

    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){};
        virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                            LogLevel::Level level, LogEvent::ptr event) = 0;
    };

private:
    std::string m_pattern;
    std::list<FormatItem::ptr> m_items;
    static std::map<std::string, std::function<LogFormatter::FormatItem::ptr(
                                     const std::string &str)>>
        format_items;
};

/**
 * @brief 日志的输出终端，在这里将stdout流或者fstream流传递给对应的formatter。
 */
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    typedef SpinLock MutexType;

    enum LogAppenderType { UNKNOWN, FILE, STD };

    virtual ~LogAppender() {}
    virtual LogAppenderType getType() = 0;
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     LogEvent::ptr event) = 0;

    static LogAppenderType getTypeByString(const std::string &str);
    static std::string getStringByType(LogAppenderType type);

    void setFormatter(const LogFormatter::ptr val);

    void setFormatter(const std::string &pattern);

    void setLevel(LogLevel::Level level) { m_level = level; };
    LogFormatter::ptr getFormatter() const { return m_formatter; };
    bool hasFormatter() const { return m_has_formattern; };

protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    LogFormatter::ptr m_formatter;
    bool m_has_formattern = false;
    MutexType m_mutex;
};
//输出到控制台的Appender
class StdOutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdOutLogAppender> ptr;
    LogAppenderType getType() override { return FILE; }
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
             LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender {
public:
    FileLogAppender(const std::string &path);
    typedef std::shared_ptr<FileLogAppender> ptr;
    LogAppenderType getType() override { return FILE; }
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
             LogEvent::ptr event) override;

    bool reopen();

private:
    std::string m_fpath;
    std::ofstream m_fstream;
};

//日志输出器
class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef SpinLock MutexType;

    Logger(const std::string &name = "root");

    /**
     * @brief 修改默认的格式器
     *
     * @param formatter 格式器
     */
    void setDefaultFormatter(LogFormatter::ptr formatter);
    void setDefaultFormatter(const std::string &str);
    /**
     * @brief 修改所有输出端的格式器
     *
     * @param formatter 格式器
     */
    void setFormatter(LogFormatter::ptr formatter);
    void setFormatter(const std::string &str);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();

    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    LogLevel::Level getLevel() const { return m_level; };
    const std::string &getName() const { return m_name; };
    void setLevel(LogLevel::Level val) { m_level = val; };

private:
    std::string m_name;
    LogLevel::Level m_level; //满足该级别的日志才会被输出
    /**
     * @m_formatter
     * 默认的formatter，如果添加的终端没有设置formatter就为其添加默认的formatter
     */
    LogFormatter::ptr m_formatter;
    /**
     * @m_appenders 输出终端列表
     */
    std::list<LogAppender::ptr> m_appenders;

    MutexType m_mutex;
};

/**
 * @brief Logger的管理类
 */
class LoggerManager {
public:
    typedef std::shared_ptr<LoggerManager> ptr;
    typedef SpinLock MutexType;
    /**
     * @brief 通过名称获取日志器，没有时则自动创建
     *
     * @param name 日志器名
     */
    Logger::ptr getLogger(const std::string &name);
    /**
     * @brief 获取根日志器，没有则自动创建
     *
     */
    Logger::ptr getRoot();

    // void setLoggersFromConfig();

private:
    std::unordered_map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_root_logger;
    MutexType m_mutex;
};

typedef Singleton<LoggerManager> LoggerMgr;

} // namespace lyon

#endif
