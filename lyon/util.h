#ifndef __LYON_UTIL_H__
#define __LYON_UTIL_H__
#include <stdint.h>
#include <thread>
namespace lyon {

pid_t GetThreadId();

uint32_t GetFiberId();

static const std::string &GetThreadName();

} // namespace lyon

#endif // !__LYON_UTIL_H__
