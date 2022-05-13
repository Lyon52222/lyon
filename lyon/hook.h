#ifndef __LYON_HOOK_H__
#define __LYON_HOOK_H__
namespace lyon {
bool is_hook_enable();
void set_hook_enable(bool flag);

extern "C" {
//这是声明一个函数类型sleep_fun
typedef unsigned int (*sleep_fun)(unsigned int seconds);
//这是声明一个该类型的变量，sleep_f
extern sleep_fun sleep_f;
}

} // namespace lyon
#endif // !__LYON_HOOK_H__
