#include "log.h"
#include <memory>
#include <sstream>

namespace lyon {
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
    // TODO:添加解析
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

class MessageFormatItem : public LogFormatter::FormatItem {
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
    void format(std::ostream &os, std::shared_ptr<Logger> logger,
                LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::toString(event->getLevel());
    }
};
} // namespace lyon
