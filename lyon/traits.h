#ifndef __LYON_TRAITS_H__
#define __LYON_TRAITS_H__

#include <functional>
namespace lyon {
template <typename F> struct function_traits;

// 普通函数
template <typename Ret, typename... Args> struct function_traits<Ret(Args...)> {
    enum { arity = sizeof...(Args) };
    typedef Ret function_type(Args...);
    typedef Ret return_type;
    using stl_function_type = std::function<function_type>;
    typedef Ret (*pointer)(Args...);

    using args_type =
        std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;

    template <size_t I> struct args {
        static_assert(I < arity,
                      "index is out of range, inde must less than sizeof Args");

        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

// 函数指针
template <typename Ret, typename... Args>
struct function_traits<Ret (*)(Args...)> : function_traits<Ret(Args...)> {};

// std::function
template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>
    : function_traits<Ret(Args...)> {};

// member function
#define FUNCION_TRAITS(...)                                                    \
    template <typename ReturnType, typename ClassType, typename... Args>       \
    struct function_traits<ReturnType (ClassType::*)(Args...) __VA_ARGS__>     \
        : function_traits<ReturnType(Args...)> {};

FUNCION_TRAITS()
FUNCION_TRAITS(const)
FUNCION_TRAITS(volatile)
FUNCION_TRAITS(const volatile)

// 函数对象
template <typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};

} // namespace lyon
#endif
