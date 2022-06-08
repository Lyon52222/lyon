#ifndef __LYON_SINGLETON_H__
#define __LYON_SINGLETON_H__
#include <memory>

namespace lyon {
/**
 * @brief 单例模式的封装类
 *
 * @tparam T 单例类
 */
template <typename T> class Singleton {
public:
    static T *GetInstance() {
        static T instance;
        return &instance;
    }
};

template <typename T> class SingletonPtr {
public:
    static std::shared_ptr<T> GetInstalce() {
        static std::shared_ptr<T> instance(new T);
        return instance;
    }
};
} // namespace lyon
#endif
