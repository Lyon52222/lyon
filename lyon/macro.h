#ifndef __LYON_MACRO_H__
#define __LYON_MACRO_H__
#include "log.h"
#include "util.h"
#include <assert.h>

/**
 * CPU会预先取下一条指令从而减少cpu等待取指令的耗时。
 * 不过当遇到分支结构跳转时预先取得的指令就没用了。
 * 因此程序源可以使用__builtin_expect 来告诉编译器大概率的走向，从而优化。
 */
#if defined __GNUC__ || defined __llvm__
// x大概率为真
#define LYON_LIKELY(x) __builtin_expect(!!(x), 1)
// x大概率为假
#define LYON_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define LYON_LIKELY(x) (x)
#define LYON_UNLIKELY(x) (x)
#endif

#define LYON_ASSERT(x)                                                         \
    if (LYON_UNLIKELY(!(x))) {                                                 \
        LYON_LOG_ERROR(LYON_LOG_GET_ROOT())                                    \
            << std::endl                                                       \
            << "ASSERTION: " #x << "\nbacktrace:\n"                            \
            << lyon::BackTraceToString(20, 2, "  ");                           \
        assert(x);                                                             \
    }

#define LYON_ASSERT2(x, m)                                                     \
    if (LYON_UNLIKELY(!(x))) {                                                 \
        LYON_LOG_ERROR(LYON_LOG_GET_ROOT())                                    \
            << std::endl                                                       \
            << m << std::endl                                                  \
            << "ASSERTION: " #x << "\nbacktrace:\n"                            \
            << lyon::BackTraceToString(20, 2, "  ");                           \
        assert(x);                                                             \
    }
#endif // !__LYON_MACRO_H__
