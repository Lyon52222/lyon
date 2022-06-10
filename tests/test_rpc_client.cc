#include "lyon/rpc/rpc_client.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/rpc/rpc_result.h>

void test_client() {
    lyon::rpc::RpcClient::ptr rpc_client(new lyon::rpc::RpcClient(1000));
    if (rpc_client->connect("localhost:8088")) {
        lyon::rpc::RpcResult<int> rt = rpc_client->call<int>("add", 123, 89);
        std::cout << rt.getMsg() << std::endl;
        std::cout << rt.getVal() << std::endl;

        lyon::rpc::RpcResult<int> rt1 = rpc_client->call<int>("add", 123, 89);
        std::cout << rt1.getMsg() << std::endl;
        std::cout << rt1.getVal() << std::endl;

        lyon::rpc::RpcResult<int> rt2 = rpc_client->call<int>("addf", 123, 89);
        std::cout << rt2.getMsg() << std::endl;
        std::cout << rt2.getVal() << std::endl;

        // TODO:这里还存在非法参数也会被成功解析的问题
        lyon::rpc::RpcResult<int> rt3 = rpc_client->call<int>("add", "ves", 89);
        std::cout << rt3.getMsg() << std::endl;
        std::cout << rt3.getVal() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    iom.addJob(test_client);
    return 0;
}
