#include "lyon/rpc/rpc_method.h"
#include "lyon/rpc/rpc_server.h"
#include <memory>

void test_regist() {
    auto add = [](int a, int b) { return a + b; };

    lyon::rpc::RpcServer::ptr rpc_server(new lyon::rpc::RpcServer());

    rpc_server->registMethod("add", add);
    lyon::Serializer ser;
    ser << 4 << 6;
    ser.setPosition(0);
    bool rt = rpc_server->getMethod("add")->call(ser);

    int ans;
    ser >> ans;

    std::cout << ans << std::endl;
}

int main(int argc, char *argv[]) {
    test_regist();
    return 0;
}
