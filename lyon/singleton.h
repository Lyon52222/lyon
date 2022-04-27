#ifndef __LYON_SINGLETON_H__
#define __LYON_SINGLETON_H__
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
#endif
