#ifndef __LYON_RPC_METHOD_H__
#define __LYON_RPC_METHOD_H__
#include "lyon/traits.h"
#include <bits/utility.h>
#include <lyon/serialize/serializer.h>
#include <memory>
#include <string>
namespace lyon::rpc {

class IRPCMethod {
public:
  typedef std::shared_ptr<IRPCMethod> ptr;

  IRPCMethod(const std::string &name) : m_name(name) {}

  virtual ~IRPCMethod();

  virtual Serializer &operator()(Serializer &ser) = 0;

  virtual const std::string &getName() const { return m_name; }

protected:
  std::string m_name;
};

template <class F> class RPCMethod : public IRPCMethod {
public:
  typedef std::shared_ptr<RPCMethod> ptr;

  RPCMethod(const std::string &name, F func) : IRPCMethod(name) {
    m_func = func;
  }

  virtual Serializer &operator()(Serializer &ser) override {
    //获得参数类型
    using Args = typename function_traits<F>::args_type;
    //获得返回值类型
    using Ret = typename function_traits<F>::return_type;
    Args args;
    ser >> args;

    constexpr auto size =
        std::tuple_size<typename std::decay<Args>::type>::value;
    const auto &unpack =
        [ this, &args ]<std::size_t... Index>(std::index_sequence<Index...>) {
      return m_func(std::get<Index>(std::forward<Args>(args))...);
    };

    Ret rt = unpack(std::make_index_sequence<size>{});

    ser.setPosition(0);
    ser << rt;
    return ser;
  }

private:
  F m_func;
};

} // namespace lyon::rpc

#endif
