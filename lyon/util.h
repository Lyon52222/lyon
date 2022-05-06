#ifndef __LYON_UTIL_H__
#define __LYON_UTIL_H__
#include <pthread.h>
#include <stdint.h>
#include <string>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>
namespace lyon {

uint64_t GetThreadId();

uint32_t GetFiberId();

inline bool IsConfigNameAvilable(const std::string &name) {
    return name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ"
                                  "RSTUVWXYZ_.0123456789") == std::string::npos;
}

} // namespace lyon

#endif // !__LYON_UTIL_H__
