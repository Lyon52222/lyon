#include "lyon/rpc/rpc_method.h"
#include "lyon/rpc/rpc_server.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <memory>
#include <sys/socket.h>

void test_regist() {
    auto add = [](int a, int b) {
        std::cout << "a" << a << std::endl << "b" << b << std::endl;
        return a + b;
    };

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

void test_server() {
    auto add = [](int a, int b) { return a + b; };

    lyon::rpc::RpcServer::ptr rpc_server(new lyon::rpc::RpcServer());

    rpc_server->registMethod("add", add);

    lyon::Address::ptr addr =
        lyon::Address::LookUpAny("0.0.0.0:8088", AF_INET, SOCK_STREAM);
    rpc_server->bindAndListen(addr);
    rpc_server->start();
}

int main(int argc, char *argv[]) {
    // test_regist();
    lyon::IOManager iom(1);
    iom.addJob(test_server);
    return 0;
}
