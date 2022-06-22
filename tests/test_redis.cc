#include "lyon/db/redis.h"
#include <lyon/log.h>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

void test_get() {
    auto reply = lyon::db::RedisUtil::Cmd("local", "get abc");
    if (!reply) {
        LYON_LOG_INFO(g_logger) << "redis get abc error";

    } else {
        LYON_LOG_INFO(g_logger)
            << "redis get abc: " << (reply->str ? reply->str : "null");
    }
}

int main(int argc, char *argv[]) {
    test_get();
    return 0;
}
