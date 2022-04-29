#include "util.h"

namespace lyon {
pid_t GetThreadId() { return 0; }
uint32_t GetFiberId() { return 0; }

inline bool IsConfigNameAvilable(const std::string &name) {
    return name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ"
                                  "RSTUVWXYZ_0123456789") == std::string::npos;
}
} // namespace lyon
