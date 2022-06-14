#include "redis.h"
#include <hiredis/hiredis.h>
#include <lyon/log.h>
namespace lyon::db {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

bool Redis::connect() { return true; }

bool Redis::connect(const std::string &host, uint16_t port, uint64_t ms) {

    return true;
}

bool Redis::reconnect() { return true; }

ReplyRpr Redis::cmd(const char *fmt, ...) { return nullptr; }

ReplyRpr Redis::cmd(const char *fmt, va_list ap) {
    auto reply = (redisReply *)redisvCommand(m_context.get(), fmt, ap);
    return nullptr;
}

ReplyRpr Redis::cmd(const char *fmt, const std::vector<std::string> &args) {

    return nullptr;
}

int Redis::appendCmd(const char *fmt, ...) { return 0; }

int Redis::appendCmd(const char *fmt, va_list ap) { return 0; }

int Redis::appendCmd(const char *fmt, const std::vector<std::string> &args) {

    return 0;
}

} // namespace lyon::db
