#ifndef __LYON_UTIL_H__
#define __LYON_UTIL_H__
#include <cstdint>
#include <pthread.h>
#include <stdint.h>
#include <string>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>
#include <vector>
namespace lyon {

/**
 * @brief 获取线程ID
 *
 * @return 线程ID
 */
uint64_t GetCurrentThreadId();

/**
 * @brief 获取协程ID
 *
 * @return 协程ID
 */
uint32_t GetCurrentFiberId();

/**
 * @brief 查询函数的调用栈
 *
 * @param bt 调用栈的保存vector
 * @param size 查询的调用深度
 * @param skip 跳过深度
 */
void BackTrace(std::vector<std::string> &bt, int size = 64, int skip = 1);

/**
 * @brief 将函数调用栈转化为string
 *
 * @param size 调用栈的查询深度
 * @param skip 查询的跳过深度
 * @param prefix 调用栈的字符串前缀
 */
std::string BackTraceToString(int size = 64, int skip = 2,
                              const std::string &prefix = " ");

/**
 * @brief 检查配置名的有效性
 *
 * @param name 配置名
 * @return 是否有效
 */
bool IsConfigNameAvilable(const std::string &name);

/**
 * @brief 获取当前事件的毫秒级时间
 *
 * @return 毫秒级表示的时间
 */

uint64_t GetCurrentTimeMS();

uint64_t GetCurrentTimeUS();

} // namespace lyon

#endif // !__LYON_UTIL_H__
