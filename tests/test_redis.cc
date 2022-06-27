#include "lyon/db/redis.h"
#include <lyon/config.h>
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_redis() {
    lyon::db::Redis redis("test");
    // redis.setCmdTimeout(100);

    if (redis.connect("127.0.0.1", 6379, 1000)) {
        LYON_LOG_INFO(g_logger) << "redis connect success";
    }

    auto reply = redis.cmd("GET test");
    if (reply) {
        LYON_LOG_INFO(g_logger)
            << "redis get test: " << (reply->str ? reply->str : "null");
    }
}

void test_get() {
    lyon::Config::LoadFromConfigFile("/home/lyon/code/C++/lyon/conf/redis.yml");
    auto reply = lyon::db::RedisUtil::Cmd("local", "get abc");
    if (!reply) {
        LYON_LOG_INFO(g_logger) << "redis get abc error";

    } else {
        LYON_LOG_INFO(g_logger)
            << "redis get abc: " << (reply->str ? reply->str : "null");
    }
}

int main(int argc, char *argv[]) {
    // test_redis();
    test_get();
    return 0;
}
