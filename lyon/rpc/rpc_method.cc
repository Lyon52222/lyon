#include "rpc_method.h"
#include <sstream>
namespace lyon::rpc {

RpcMethodMeta::RpcMethodMeta(const std::string &name, const std::string &desc)
    : m_name(name), m_description(desc) {}

RpcMethodMeta::RpcMethodMeta(const std::string &name,
                             const std::string &rt_type,
                             const std::string &args_type,
                             const std::string &desc)
    : m_name(name), m_rt_type(rt_type), m_args_type(args_type),
      m_description(desc) {}

std::ostream &RpcMethodMeta::dump(std::ostream &os) const {
    os << "[ func_name = " << m_name << ", rt_type = " << m_rt_type
       << ", args_type = " << m_args_type << ", desc = " << m_description
       << "]";
    return os;
}
std::string RpcMethodMeta::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

bool RpcMethodMeta::operator<(const RpcMethodMeta &rhs) const {
    if (m_name != rhs.m_name) {
        return m_name < rhs.m_name;
    } else if (m_rt_type != rhs.m_rt_type) {
        return m_rt_type < rhs.m_rt_type;
    } else {
        return m_args_type < rhs.m_args_type;
    }
}

bool RpcMethodMeta::operator==(const RpcMethodMeta &rhs) const {
    return m_name == rhs.m_name && m_rt_type == rhs.m_rt_type &&
           m_args_type == rhs.m_args_type;
}

bool RpcMethodMeta::operator!=(const RpcMethodMeta &rhs) const {
    return !(*this == rhs);
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
