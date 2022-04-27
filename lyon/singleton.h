#ifndef __LYON_SINGLETON_H__
#define __LYON_SINGLETON_H__
#include <memory>

namespace lyon {
/**
 * @brief 单例模式的封装类
 *
 * @tparam T 单例类
 */
template <class T> class Singleton {
  public:
    static T *getInstance() {
        static T instance;
        return &instance;
    }
};

template <class T> class SingletonPtr {
  public:
    static std::shared_ptr<T> getInstance() {
        static std::shared_ptr<T> instance(new T);
        return instance;
    }
};
} // namespace lyon
#endif
