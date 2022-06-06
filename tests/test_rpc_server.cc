#include "lyon/rpc/rpc_method.h"
#include "lyon/rpc/rpc_server.h"
#include <memory>

void test() {
    auto add = [](int a, int b) { return a + b; };

    lyon::rpc::RPCServer::ptr rpc_server(new lyon::rpc::RPCServer());
    rpc_server->registMethod(
        std::make_shared<lyon::rpc::RPCMethod<decltype(add)>>("add", add));

    lyon::Serializer ser;
    ser << 4 << 6;
    // rpc_server->getMethod("add")->(ser);
}

int main(int argc, char *argv[]) { return 0; }
