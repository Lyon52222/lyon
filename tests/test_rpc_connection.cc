#include "lyon/rpc/rpc_connection.h"
#include <future>
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/log.h>
#include <lyon/rpc/rpc_connection_poll.h>
#include <lyon/rpc/rpc_result.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_call() {
    lyon::rpc::RpcConnection::ptr rpc_client(new lyon::rpc::RpcConnection());
    //支持函数名重载
    if (rpc_client->connect("localhost:8088")) {
        lyon::rpc::RpcResult<int> rt = rpc_client->call<int>("add", 123, 89);
        LYON_LOG_INFO(g_logger) << rt.getMsg();
        LYON_LOG_INFO(g_logger) << rt.getVal();

        lyon::rpc::RpcResult<float> rtf =
            rpc_client->call<float>("add", 12.2f, 89.1f);
        LYON_LOG_INFO(g_logger) << rtf.getMsg();
        LYON_LOG_INFO(g_logger) << rtf.getVal();
        //找不到方法名没有问题
        // lyon::rpc::RpcResult<int> rt2 = rpc_client->call<int>("addf", 123,
        // 89); LYON_LOG_INFO(g_logger) << rt2.getMsg(); LYON_LOG_INFO(g_logger)
        // << rt2.getVal();

        //测试通过
        // lyon::rpc::RpcResult<int> rt3 = rpc_client->call<int>("add", "ves",
        // 89); LYON_LOG_INFO(g_logger) << rt3.getMsg(); LYON_LOG_INFO(g_logger)
        // << rt3.getVal();
    }
}

void call_back(lyon::rpc::RpcResult<int> result) {
    LYON_LOG_INFO(g_logger) << result.getMsg();
    LYON_LOG_INFO(g_logger) << result.getVal();
};

void test_async_call() {
    lyon::rpc::RpcConnection::ptr rpc_client(
        new lyon::rpc::RpcConnection(1000));
    if (rpc_client->connect("localhost:8088")) {
        rpc_client->async_call<int>(call_back, "add", 1, 3);
    }
}

void test_future_call() {
    lyon::rpc::RpcConnection::ptr rpc_client(
        new lyon::rpc::RpcConnection(1000));
    if (rpc_client->connect("localhost:8088")) {
        auto future = rpc_client->future_call<int>("add", 1, 3);

        std::future_status status;
        do {
            status = future.wait_for(std::chrono::seconds(1));
            if (status == std::future_status::deferred) {
                LYON_LOG_INFO(g_logger) << "deferred\n";
            } else if (status == std::future_status::timeout) {
                LYON_LOG_INFO(g_logger) << "timeout\n";
            } else if (status == std::future_status::ready) {
                LYON_LOG_INFO(g_logger) << "ready!\n";

                auto result = future.get();

                LYON_LOG_INFO(g_logger) << result.getMsg();
                LYON_LOG_INFO(g_logger) << result.getVal();

                break;
            }
        } while (status != std::future_status::ready);
    }
}

void test_poll_call() {
    lyon::rpc::RpcConnectionPoll::ptr poll =
        lyon::rpc::RpcConnectionPoll::Create(20, 10000, 20);
    poll->bindRegister("localhost:8089");

    lyon::rpc::RpcResult<int> rt = poll->call<int>("add", 123, 89);
    LYON_LOG_INFO(g_logger) << rt.getMsg();
    LYON_LOG_INFO(g_logger) << rt.getVal();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(2);
    // iom.addJob(test_call);
    // iom.addJob(test_async_call);
    // iom.addJob(test_future_call);

    iom.addJob(test_poll_call);
    return 0;
}
