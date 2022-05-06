#include "log.h"
#include "config.h"
#include "utils/file_system_util.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <yaml-cpp/node/parse.h>

namespace lyon {

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string &str = "") : m_str(str) {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << m_str;
    }

private:
    std::string m_str;
};

class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::toString(event->getLevel());
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

class DataTimeFormatItem : public LogFormatter::FormatItem {
public:
    DataTimeFormatItem(const std::string &format = "%Y-%m-%d %H-%M-%S")
        : m_format(format) {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }

private:
    std::string m_format;
};

class FileNameFormatItem : public LogFormatter::FormatItem {
public:
    FileNameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class FileLineFormatItem : public LogFormatter::FormatItem {
public:
    FileLineFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getLine();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadName();
    }
};

class NewLineFomatItem : public LogFormatter::FormatItem {
public:
    NewLineFomatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class TabFomatItem : public LogFormatter::FormatItem {
public:
    TabFomatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << '\t';
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getFiberId();
    }
};

class LoggerNameFormatItem : public LogFormatter::FormatItem {
public:
    LoggerNameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getLogger()->getName();
    }
};

LogEvent::LogEvent(std::shared_ptr<Logger> logger, const char *file,
                   int32_t line, uint64_t threadId,
                   const std::string &threadName, uint32_t fiberId,
                   uint64_t time, uint32_t elapse, LogLevel::Level level)
    : m_logger(logger), m_file(file), m_line(line), m_threadId(threadId),
      m_threadName(threadName), m_fiberId(fiberId), m_time(time),
      m_elapse(elapse), m_level(level) {}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

void Logger::setDefaultFormatter(LogFormatter::ptr formatter) {
    m_formatter = formatter;
}

void Logger::setDefaultFormatter(const std::string &str) {
    m_formatter->setPattern(str);
}

Logger::Logger(const std::string &name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::addAppender(LogAppender::ptr appender) {
    if (!appender->hasFormatter()) {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    auto itr = std::find(m_appenders.begin(), m_appenders.end(), appender);
    if (itr != m_appenders.end()) {
        m_appenders.erase(itr);
    }
}

void Logger::clearAppenders() { m_appenders.clear(); }

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        for (auto appender : m_appenders) {
            appender->log(self, level, event);
        }
    }
}
void Logger::debug(LogEvent::ptr event) { log(LogLevel::DEBUG, event); }
void Logger::info(LogEvent::ptr event) { log(LogLevel::INFO, event); }
void Logger::warn(LogEvent::ptr event) { log(LogLevel::WARN, event); }
void Logger::error(LogEvent::ptr event) { log(LogLevel::ERROR, event); }
void Logger::fatal(LogEvent::ptr event) { log(LogLevel::FATAL, event); }

LogAppender::LogAppenderType
LogAppender::getTypeByString(const std::string &str) {
    if (str == "FileLogAppender") {
        return FILE;
    } else if (str == "StdOutLogAppender") {
        return STD;
    } else {
        return UNKNOWN;
    }
}

std::string LogAppender::getStringByType(LogAppender::LogAppenderType type) {
    if (type == FILE) {
        return "FileLogAppender";
    } else if (type == STD) {
        return "StdOutLogAppender";
    } else {
        return "unknown";
    }
}

FileLogAppender::FileLogAppender(const std::string &path) : m_fpath(path) {
    reopen();
}

bool FileLogAppender::reopen() {
    if (m_fstream) {
        m_fstream.close();
    }
    FSUtil::OpenForWrite(m_fstream, m_fpath, std::ios::app);
    if (!m_fstream.is_open()) {
        LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
            << "FileLogAppender: open " << m_fpath << " failed!";
        return false;
    }
    return true;
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                          LogEvent::ptr event) {
    if (m_fstream.is_open() && level >= m_level) {
        m_formatter->format(m_fstream, logger, level, event);
    }
}

void StdOutLogAppender::log(std::shared_ptr<Logger> logger,
                            LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_formatter->format(std::cout, logger, level, event);
    }
}

const char *LogLevel::toString(LogLevel::Level level) {
    switch (level) {
#define XX(name)                                                               \
    case name:                                                                 \
        return #name;
        XX(UNKNOWN)
        XX(DEBUG)
        XX(INFO)
        XX(WARN)
        XX(ERROR)
        XX(FATAL)
#undef XX
    default:
        return "UNKNOWN";
    }
    return "UNKNOWN";
}

LogLevel::Level LogLevel::fromString(const std::string &level) {
#define XX(name)                                                               \
    if (level == #name)                                                        \
        return LogLevel::name;
    XX(UNKNOWN)
    XX(DEBUG)
    XX(INFO)
    XX(WARN)
    XX(ERROR)
    XX(FATAL)
#undef XX
    return LogLevel::UNKNOWN;
}

LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern) {
    parsePattern();
}

void LogFormatter::setPattern(std::string pattern) {
    m_pattern = pattern;
    parsePattern();
}

void LogFormatter::parsePattern() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string sstr;
    for (size_t i = 0; i < m_pattern.size(); i++) {
        if (m_pattern[i] != '%') {
            sstr.append(1, m_pattern[i]);
            continue;
        }
        if (i + 1 < m_pattern.size() && m_pattern[i + 1] == '%') {
            sstr.append(1, '%');
            i++;
            continue;
        }
        if (!sstr.empty()) {
            vec.emplace_back(sstr, std::string(), 0);
            sstr.clear();
        }
        short fmt_status = 0;
        size_t fmt_start = 0;
        size_t j = i + 1;
        std::string fmt;
        while (j < m_pattern.size()) {
            if (!fmt_status && !std::isalpha(m_pattern[j]) &&
                m_pattern[j] != '{' && m_pattern[j] != '}') {
                sstr = m_pattern.substr(i + 1, j - i - 1);
                break;
            }

            if (fmt_status == 0 && m_pattern[j] == '{') {
                sstr = m_pattern.substr(i + 1, j - i - 1);
                fmt_status = 1;
                fmt_start = j + 1;
            } else if (fmt_status == 1 && m_pattern[j] == '}') {
                fmt = m_pattern.substr(fmt_start, j - fmt_start);
                fmt_status = 2;
                j++;
                break;
            }
            j++;
            if (j == m_pattern.size()) {
                if (sstr.empty()) {
                    sstr = m_pattern.substr(i + 1);
                }
            }
        }
        if (fmt_status == 0) {
            vec.emplace_back(sstr, std::string(), 1);
            sstr.clear();
        } else if (fmt_status == 2) {
            vec.emplace_back(sstr, fmt, 2);
            sstr.clear();
        } else {
            std::cout << "pattern parse error:" << m_pattern << '-'
                      << m_pattern.substr(i) << std::endl;
            vec.emplace_back("<pattern erroe>", fmt, 0);
        }
        i = j - 1;
    }

    if (!sstr.empty()) {
        vec.emplace_back(sstr, std::string(), 0);
    }

    for (size_t i = 0; i < vec.size(); i++) {
        int item_type = std::get<2>(vec[i]);
        if (item_type == 0) {
            m_items.push_back(
                FormatItem::ptr(new StringFormatItem(std::get<0>(vec[i]))));
        } else {
            auto itr = format_items.find(std::get<0>(vec[i]));
            if (itr != format_items.end()) {
                if (item_type == 1) {
                    m_items.push_back(itr->second(""));
                } else if (item_type == 2) {
                    m_items.push_back(itr->second(std::get<1>(vec[i])));
                } else {
                }
            }
        }
    }
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger,
                                 LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto &item : m_items) {
        item->format(ss, logger, level, event);
    }
    return ss.str();
}

std::ostream &LogFormatter::format(std::ostream &os,
                                   std::shared_ptr<Logger> logger,
                                   LogLevel::Level level, LogEvent::ptr event) {
    for (auto &item : m_items) {
        item->format(os, logger, level, event);
    }
    return os;
}

std::map<std::string,
         std::function<LogFormatter::FormatItem::ptr(const std::string &str)>>
    LogFormatter::format_items = {
#define XX(str, C)                                                             \
    {                                                                          \
#str,                                                                  \
            [](const std::string &fmt) { return FormatItem::ptr(new C(fmt)); } \
    }

        XX(m, MessageFormatItem),    XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),     XX(t, ThreadIdFormatItem),
        XX(d, DataTimeFormatItem),   XX(f, FileNameFormatItem),
        XX(l, FileLineFormatItem),   XX(n, NewLineFomatItem),
        XX(T, TabFomatItem),         XX(F, FiberIdFormatItem),
        XX(N, ThreadNameFormatItem), XX(c, LoggerNameFormatItem),

#undef XX
        // {"p", [](const std::string &fmt) {
        //      return FormatItem::ptr(new LevelFormatItem(fmt));
        //  }}
};

/**
 * struct ConfigAppender - Config文件中appender对应的解析类
 */
struct ConfigAppender {
    LogAppender::LogAppenderType type;
    std::string file;
    LogLevel::Level level = LogLevel::UNKNOWN;
    std::string formatter;

    bool operator==(const ConfigAppender &oth) const {
        return type == oth.type && file == oth.file && level == oth.level &&
               formatter == oth.formatter;
    }
};

/**
 * @brief 将string转化为ConfigAppender
 */
template <> class LexicalCast<std::string, ConfigAppender> {
public:
    ConfigAppender operator()(const std::string &s) {
        YAML::Node app = YAML::Load(s);
        if (!app["type"].IsDefined()) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "Config appender must have a type!";
            throw std::logic_error("Config appender must have a type!");
        }
        ConfigAppender appender;
        LogAppender::LogAppenderType apptype =
            LogAppender ::getTypeByString(app["type"].as<std::string>());
        if (apptype == LogAppender::UNKNOWN) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "Config appender type - " << app["type"].as<std::string>()
                << " unknown!";
            throw std::logic_error("Config appender type unknown!");
        } else {
            appender.type = apptype;
        }

        if (appender.type == LogAppender::FILE && (!app["file"].IsDefined())) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "Config appender type FileLogAppender must indicate it's "
                   "file!";
            throw std::logic_error(
                "Config appender type FileLogAppender must indicate it's "
                "file!");
        }

        if (app["file"].IsDefined())
            appender.file = app["file"].as<std::string>();
        if (app["level"].IsDefined())
            appender.level =
                LogLevel::fromString(app["level"].as<std::string>());
        if (app["formatter"].IsDefined())
            appender.formatter = app["formatter"].as<std::string>();
        return appender;
    }
};

template <> class LexicalCast<ConfigAppender, std::string> {
public:
    std::string operator()(const ConfigAppender &appender) {
        YAML::Node node;
        node["type"] = LogAppender::getStringByType(appender.type);
        node["file"] = appender.file;
        node["level"] = LogLevel::toString(appender.level);
        node["formatter"] = appender.formatter;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * struct ConfigLogger Config文件中logger对应的解析类
 */
struct ConfigLogger {
    std::string name;
    LogLevel::Level level = LogLevel::UNKNOWN;
    std::string formatter;
    std::vector<ConfigAppender> appenders;

    bool operator==(const ConfigLogger &oth) const {
        return name == oth.name && level == oth.level &&
               formatter == oth.formatter && appenders == oth.appenders;
    }

    bool operator<(const ConfigLogger &oth) const { return name < oth.name; }
};

/**
 * @brief 将string转化为ConfigLogger
 */
template <> class LexicalCast<std::string, ConfigLogger> {
public:
    ConfigLogger operator()(const std::string &s) {
        YAML::Node log = YAML::Load(s);

        if (!log["name"].IsDefined()) {
            LYON_LOG_ERROR(LYON_LOG_GET_ROOT())
                << "Config logs must have a name!";
            throw std::logic_error("Config logs must have a name!");
        }
        ConfigLogger logger;
        logger.name = log["name"].as<std::string>();
        if (log["level"].IsDefined())
            logger.level = LogLevel::fromString(log["level"].as<std::string>());
        if (log["formatter"].IsDefined())
            logger.formatter = log["formatter"].as<std::string>();
        if (log["appenders"].IsDefined()) {
            std::stringstream ss;
            ss << log["appenders"];
            logger.appenders =
                LexicalCast<std::string, std::vector<ConfigAppender>>()(
                    ss.str());
        }

        return logger;
    }
};

/**
 * @brief 将ConfigLogger转化为string
 */
template <> class LexicalCast<ConfigLogger, std::string> {
public:
    std::string operator()(const ConfigLogger &logger) {
        YAML::Node node;
        node["name"] = logger.name;
        node["level"] = LogLevel::toString(logger.level);
        node["formatter"] = logger.formatter;

        for (size_t i = 0; i < logger.appenders.size(); i++) {
            node["appenders"].push_back(
                LexicalCast<ConfigAppender, std::string>()(
                    logger.appenders[i]));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 全局变量 logs的Config变量。用于在配置中添加一条空的logs记录
 *
 */
ConfigVar<std::set<ConfigLogger>>::ptr g_log_defines = lyon::Config::SetConfig(
    "logs", std::set<lyon::ConfigLogger>{}, "config of logs");

struct LogConfigInit {
    LogConfigInit() {
        g_log_defines->addOnChange([](const std::set<ConfigLogger> &old_val,
                                      const std::set<ConfigLogger> &new_val) {
            Logger::ptr logger;
            for (auto itr_new = new_val.cbegin(); itr_new != new_val.cend();
                 itr_new++) {
                const auto itr_old = old_val.find(*itr_new);
                if (itr_old == old_val.end()) {
                    logger = LYON_LOG_GET_LOGGER(itr_new->name);
                } else {
                    if ((*itr_old) == (*itr_new)) {
                        continue;
                    } else {
                        logger = LYON_LOG_GET_LOGGER(itr_new->name);
                    }
                }
                if (itr_new->level != LogLevel::UNKNOWN)
                    logger->setLevel((*itr_new).level);
                if (!(itr_new->formatter).empty())
                    logger->setDefaultFormatter(itr_new->formatter);

                logger->clearAppenders();
                for (auto &app : itr_new->appenders) {
                    LogAppender::ptr appender;
                    if (app.type == LogAppender::FILE) {
                        appender.reset(new FileLogAppender(app.file));
                    } else if (app.type == LogAppender::STD) {
                        appender.reset(new StdOutLogAppender());
                    }
                    if (app.level != LogLevel::UNKNOWN)
                        appender->setLevel(app.level);
                    if (!(app.formatter.empty()))
                        appender->setFormatter(app.formatter);
                    logger->addAppender(appender);
                }
            }

            for (auto &itr_old : old_val) {
                const auto itr_new = new_val.find(itr_old);
                if (itr_new == new_val.end()) {
                    auto logger = LYON_LOG_GET_LOGGER(itr_old.name);
                    logger->setLevel(LogLevel::UNKNOWN);
                    logger->clearAppenders();
                }
            }

            // LYON_LOG_INFO(LYON_LOG_GET_LOGGER("system"))
            //     << "system logger test";
        });
    }
};

/**
 * @brief
 * Log的初始化类全局变量，用于给配置项logs添加一个回调函数用于在logs配置文件发生变化时自动同步到Config类中
 */
static LogConfigInit __log_config_init;

Logger::ptr LoggerManager::getLogger(const std::string &name) {
    auto item = m_loggers.find(name);
    if (item == m_loggers.end()) {
        auto logger = Logger::ptr(new Logger(name));
        m_loggers[name] = logger;
        return logger;
    } else {
        return item->second;
    }
}

Logger::ptr LoggerManager::getRoot() {
    if (!m_root_logger) {
        m_root_logger.reset(new Logger());
        m_root_logger->addAppender(LogAppender::ptr(new StdOutLogAppender()));
        m_loggers[m_root_logger->getName()] = m_root_logger;
    }
    return m_root_logger;
}

// void LoggerManager::setLoggersFromConfig() {}

} // namespace lyon
