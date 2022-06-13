#ifndef __LYON_RPC_METHOD_H__
#define __LYON_RPC_METHOD_H__
#include "lyon/serialize/serializer.h"
#include "lyon/traits.h"
#include <memory>
#include <string>
namespace lyon::rpc {

/**
 * @brief 方法类元数据，用于注册方法时发送数据使用
 */
class RpcMethodMeta {
public:
  typedef std::shared_ptr<RpcMethodMeta> ptr;
  RpcMethodMeta() = default;
  RpcMethodMeta(const std::string &name, const std::string &desc = "");

  friend const Serializer &operator<<(const Serializer &ser,
                                      const RpcMethodMeta &meta);

  friend const Serializer &operator>>(const Serializer &ser,
                                      RpcMethodMeta &meta);

  std::ostream &dump(std::ostream &os) const;
  std::string toString() const;

  bool operator<(const RpcMethodMeta &rhs) const;
  bool operator==(const RpcMethodMeta &rhs) const;
  bool operator!=(const RpcMethodMeta &rhs) const;

protected:
  std::string m_name;
  std::string m_rt_type;
  std::string m_args_type;
  std::string m_description;
};

/**
 * @brief 将注册的方法单独封装成一个类，这样可以添加一些额外的属性
 */
class RpcMethod : public RpcMethodMeta {
public:
  typedef std::shared_ptr<RpcMethod> ptr;

  template <typename F>
  RpcMethod(const std::string &name, F func, const std::string &desc = "")
      : RpcMethodMeta(name, desc) {
    m_func = [func, this](Serializer &args) -> bool {
      return proxy(func, args);
    };
    m_rt_type = typeid(typename function_traits<F>::return_type).name();
    m_args_type = typeid(typename function_traits<F>::args_type).name();
  }

  template <typename F> bool proxy(F func, Serializer &args) {
    using Args = typename function_traits<F>::args_type;
    //获得返回值类型
    using Ret = typename function_traits<F>::return_type;
    Args args_tuple;

    try {
      args >> args_tuple;
      args.clear();
      constexpr auto size =
          std::tuple_size<typename std::decay<Args>::type>::value;
      const auto &unpack = [&func, &args_tuple ]<std::size_t... Index>(
          std::index_sequence<Index...>) {
        return func(std::get<Index>(std::forward<Args>(args_tuple))...);
      };

      Ret rt = unpack(std::make_index_sequence<size>{});
      args << rt;
    } catch (...) {
      return false;
    };
    args.setPosition(0);
    return true;
  }

  /**
   * @brief 调用方法，args中保存了参数，调用完成后返回值保存在args中
   *
   * @param args 序列化好的参数
   * @return 调用是否成功
   */
  [[nodiscard]] bool operator()(Serializer &args) { return m_func(args); }

  [[nodiscard]] bool call(Serializer &args) { return m_func(args); }

  const std::string &getName() { return m_name; }
  const std::string &getRtType() { return m_rt_type; }
  const std::string &getArgsType() { return m_args_type; }

private:
  std::function<bool(Serializer &)> m_func;
};

} // namespace lyon::rpc

#endif
