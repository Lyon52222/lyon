#include "log.h"
#include <memory>

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
                   int32_t line, uint32_t threadId, uint32_t fiberId,
                   uint64_t time, uint32_t elapse, LogLevel::Level level)
    : m_logger(logger), m_file(file), m_line(line), m_threadId(threadId),
      m_fiberId(fiberId), m_time(time), m_elapse(elapse), m_level(level) {}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

void Logger::setDefaultFormatter(LogFormatter::ptr formatter) {
    m_formatter = formatter;
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

FileLogAppender::FileLogAppender(const std::string &name) : m_fname(name) {
    reopen();
}

bool FileLogAppender::reopen() {
    if (m_fstream) {
        m_fstream.close();
    }
    m_fstream.open(m_fname);
    return !!m_fstream;
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                          LogEvent::ptr event) {
    if (level >= m_level) {
        m_formatter->format(m_fstream, logger, level, event);
    }
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger,
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

    // for (auto &i : vec) {
    //     std::cout << "key: " << std::get<0>(i) << "\t value: " <<
    //     std::get<1>(i)
    //               << "\t type: " << std::get<2>(i) << std::endl;
    // }

    if (!sstr.empty()) {
        vec.emplace_back(sstr, std::string(), 0);
    }

    for (int i = 0; i < vec.size(); i++) {
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

std::unordered_map<std::string, std::function<LogFormatter::FormatItem::ptr(
                                    const std::string &str)>>
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
        m_root_logger->addAppender(LogAppender::ptr(new StdoutLogAppender()));
        m_loggers[m_root_logger->getName()] = m_root_logger;
    }
    return m_root_logger;
}

} // namespace lyon
