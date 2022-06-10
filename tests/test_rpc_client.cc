#include "lyon/rpc/rpc_client.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/rpc/rpc_result.h>

void test_call() {
    lyon::rpc::RpcClient::ptr rpc_client(new lyon::rpc::RpcClient(1000));
    if (rpc_client->connect("localhost:8088")) {
        lyon::rpc::RpcResult<int> rt = rpc_client->call<int>("add", 123, 89);
        std::cout << rt.getMsg() << std::endl;
        std::cout << rt.getVal() << std::endl;

        //没有问题
        lyon::rpc::RpcResult<int> rt2 = rpc_client->call<int>("addf", 123, 89);
        std::cout << rt2.getMsg() << std::endl;
        std::cout << rt2.getVal() << std::endl;

        // TODO:这里还存在非法参数也会被成功解析的问题
        lyon::rpc::RpcResult<int> rt3 = rpc_client->call<int>("add", "ves", 89);
        std::cout << rt3.getMsg() << std::endl;
        std::cout << rt3.getVal() << std::endl;
    }
}

void call_back(lyon::rpc::RpcResult<int> result) {
    std::cout << result.getMsg() << std::endl;
    std::cout << result.getVal() << std::endl;
};

void test_async_call() {
    lyon::rpc::RpcClient::ptr rpc_client(new lyon::rpc::RpcClient(1000));
    if (rpc_client->connect("localhost:8088")) {
        rpc_client->async_call<int>(call_back, "add", 1, 3);
    }
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(1);
    // iom.addJob(test_call);
    iom.addJob(test_async_call);
    return 0;
}
