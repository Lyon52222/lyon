#include "lyon/rpc/rpc_register.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>

void test() {
    auto rpc_server = std::make_shared<lyon::rpc::RpcRegister>();
    rpc_server->bindAndListen(lyon::Address::LookUpAnyIpAddress(
        "0.0.0.0:8089", AF_INET, SOCK_STREAM));

    rpc_server->start();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    iom.addJob(test);
    return 0;
}
