#ifndef __LYON_UTIL_H__
#define __LYON_UTIL_H__
#include <stdint.h>
#include <thread>
namespace lyon {

pid_t GetThreadId();

uint32_t GetFiberId();

inline bool IsConfigNameAvilable(const std::string &name) {
    return name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ"
                                  "RSTUVWXYZ_0123456789") == std::string::npos;
}
} // namespace lyon

#endif // !__LYON_UTIL_H__
