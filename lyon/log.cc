#include "log.h"
#include <cctype>
#include <functional>
#include <memory>

namespace lyon {

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
        os << event->getThreaId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
  public:
    ThreadNameFormatItem(const std::string &str = "") {}
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreaName();
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

Logger::Logger(const std::string &name) : m_name(name) {}

void Logger::addAppender(LogAppender::ptr appender) {
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

FileLogAppender::FileLogAppender(const std::string &name) : m_fname(name) {}
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

LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern) {}
void LogFormatter::init() {
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

        vec.emplace_back(sstr, std::string(), 0);
        short fmt_status = 0;
        size_t fmt_start = 0;
        size_t j = i + 1;
        std::string fmt;
        while (j < m_pattern.size()) {
            if (!fmt_status && !std::isalpha(m_pattern[j]) &&
                m_pattern[j] != '{' && m_pattern[j] != '}') {
                sstr = m_pattern.substr(i, j - i);
                break;
            }

            if (fmt_status == 0 && m_pattern[j] == '{') {
                sstr = m_pattern.substr(i, j - i);
                fmt_status = 1;
                fmt_start = j + 1;
                continue;
            } else if (fmt_status == 1 && m_pattern[j] == '}') {
                fmt = m_pattern.substr(fmt_start, j - fmt_start);
                fmt_status = 2;
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
    // TODO::add to format_items
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

        XX(m, MessageFormatItem), XX(p, LevelFormatItem),
#undef XX
        // {"p", [](const std::string &fmt) {
        //      return FormatItem::ptr(new LevelFormatItem(fmt));
        //  }}
};

} // namespace lyon
