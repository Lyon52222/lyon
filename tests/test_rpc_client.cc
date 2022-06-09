#include "lyon/rpc/rpc_client.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/rpc/rpc_result.h>

void test_client() {
    lyon::rpc::RpcClient::ptr rpc_client(new lyon::rpc::RpcClient(3000));
    lyon::Address::ptr addr =
        lyon::Address::LookUpAnyIpAddress("localhost:8088");
    if (rpc_client->connect(addr)) {
        lyon::rpc::RpcResult<int> rt = rpc_client->call<int>("add", 123, 89);
        std::cout << rt.getMsg() << std::endl;
        std::cout << rt.getVal() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    iom.addJob(test_client);
    return 0;
}
