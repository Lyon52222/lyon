#include "rpc_server.h"
namespace lyon::rpc {

void RPCServer::registMethod(IRPCMethod::ptr method) {
    m_methods[method->getName()] = method;
}

IRPCMethod::ptr RPCServer::getMethod(const std::string &name) {
    auto itr = m_methods.find(name);
    if (itr == m_methods.end()) {
        return nullptr;
    }
    return itr->second;
}

} // namespace lyon::rpc
