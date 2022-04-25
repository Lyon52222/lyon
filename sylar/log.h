#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
#include <stdint.h>
#include <string>
namespace sylar {

class LogEvent {
public:
  LogEvent();

private:
  const char *m_file = nullptr;
  int32_t m_line = 0;
  uint32_t m_threadId = 0;
};

//日志输出器
class Logger {
public:
  Logger();

private:
};

//日志格式化
class LogFormatter {};

} // namespace sylar

//日志输出地
class Appender {
public:
  virtual ~Appender() {}
};

#endif
