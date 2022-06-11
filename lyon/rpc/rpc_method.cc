#include "rpc_method.h"
#include <sstream>
namespace lyon::rpc {

RpcMethodMeta::RpcMethodMeta(const std::string &name, const std::string &desc)
    : m_name(name), m_description(desc) {}

std::ostream &RpcMethodMeta::dump(std::ostream &os) const {
    os << "[" << m_name << "," << m_rt_type << "," << m_args_type << ","
       << m_description << "]";
    return os;
}
std::string RpcMethodMeta::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

const Serializer &operator<<(const Serializer &ser, const RpcMethodMeta &meta) {
    ser << meta.m_name << meta.m_rt_type << meta.m_args_type
        << meta.m_description;
    return ser;
}

const Serializer &operator>>(const Serializer &ser, RpcMethodMeta &meta) {
    ser >> meta.m_name >> meta.m_rt_type >> meta.m_args_type >>
        meta.m_description;
    return ser;
}

} // namespace lyon::rpc
