#include "redis.h"
#include "lyon/config.h"
#include "lyon/log.h"
#include <cstdint>
#include <cstring>
#include <hiredis/hiredis.h>
#include <hiredis/read.h>
namespace lyon::db {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");
static ConfigVar<std::map<std::string, std::map<std::string, std::string>>>::ptr
    g_redis = Config::SetConfig(
        "redis.config",
        std::map<std::string, std::map<std::string, std::string>>(),
        "redis config");

static std::string get_value(const std::map<std::string, std::string> &m,
                             const std::string &key,
                             const std::string &def = "") {
    auto itr = m.find(key);
    return itr == m.end() ? def : itr->second;
}

Redis::Redis(const std::string &name) {
    m_name = name;
    m_type = REDIS;
}

Redis::Redis(const std::string &name,
             const std::map<std::string, std::string> &conf) {
    m_name = name;

    auto host_port = get_value(conf, "host", "localhost:6379");
    auto pos = host_port.find(":");
    m_host = host_port.substr(0, pos);
    m_port = atoi(host_port.substr(pos + 1).c_str());
    m_password = get_value(conf, "password");

    auto timeout_str = get_value(conf, "timeout");
    uint64_t timeout = atoi(timeout_str.c_str());
    m_cmdTimeout.tv_sec = timeout / 1000;
    m_cmdTimeout.tv_usec = timeout % 1000 * 1000;
}

bool Redis::connect() { return connect(m_host, m_port, 50); }

bool Redis::connect(const std::string &host, uint16_t port, uint64_t ms) {
    m_host = host;
    m_port = port;
    m_connectMs = ms;
    if (m_context) {
        return true;
    }

    timeval tv{(int)m_connectMs / 1000, (int)m_connectMs % 1000 * 1000};

    auto c = redisConnectWithTimeout(host.c_str(), port, tv);
    if (c) {
        if (m_cmdTimeout.tv_sec || m_cmdTimeout.tv_usec) {
            redisSetTimeout(c, m_cmdTimeout);
        }
        m_context.reset(c, redisFree);

        if (!m_password.empty()) {
            auto r = (redisReply *)redisCommand(m_context.get(), "auth %s",
                                                m_password.c_str());
            if (!r) {
                LYON_LOG_ERROR(g_logger) << "auth error:(" << m_host << ":"
                                         << m_port << ", " << m_name << ")";

                return false;
            }
            if (r->type != REDIS_REPLY_STATUS || !r->str ||
                strcmp(r->str, "OK")) {
                LYON_LOG_ERROR(g_logger) << "reply error:(" << m_host << ":"
                                         << m_port << ", " << m_name << ")";
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

bool Redis::reconnect() { return redisReconnect(m_context.get()); }

void Redis::setCmdTimeout(uint64_t to) {
    m_cmdTimeout.tv_sec = to / 1000;
    m_cmdTimeout.tv_usec = to % 1000 * 1000;
    redisSetTimeout(m_context.get(), m_cmdTimeout);
}

ReplyRpr Redis::cmd(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    ReplyRpr rt = cmd(fmt, ap);
    va_end(ap);
    return rt;
}

ReplyRpr Redis::cmd(const char *fmt, va_list ap) {
    auto r = (redisReply *)redisvCommand(m_context.get(), fmt, ap);
    if (!r) {
        LYON_LOG_ERROR(g_logger)
            << "redisCommand error: (" << fmt << ")(" << m_host << ":" << m_port
            << ")(" << m_name << ")"
            << " error: " << m_context->errstr;
        return nullptr;
    }
    ReplyRpr rt(r, freeReplyObject);
    if (rt->type == REDIS_REPLY_ERROR) {
        LYON_LOG_ERROR(g_logger)
            << "redisCommand error: (" << fmt << ")(" << m_host << ":" << m_port
            << ")(" << m_name << ")";
        return nullptr;
    }
    return rt;
}

ReplyRpr Redis::cmd(const std::vector<std::string> &argv) {
    std::vector<const char *> v;
    std::vector<size_t> l;
    for (auto &arg : argv) {
        v.push_back(arg.c_str());
        l.push_back(arg.length());
    }

    auto r = (redisReply *)redisCommandArgv(m_context.get(), argv.size(), &v[0],
                                            &l[0]);
    if (!r) {
        LYON_LOG_ERROR(g_logger) << "redisCommandArgv error: (" << m_host << ":"
                                 << m_port << ")(" << m_name << ")";
        return nullptr;
    }

    ReplyRpr rt(r, freeReplyObject);
    if (rt->type == REDIS_REPLY_ERROR) {
        LYON_LOG_ERROR(g_logger) << "redisCommandArgv error: (" << m_host << ":"
                                 << m_port << ")(" << m_name << ")";
        return nullptr;
    }
    return rt;
}

int Redis::appendCmd(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int rt = appendCmd(fmt, ap);
    va_end(ap);
    return rt;
}

int Redis::appendCmd(const char *fmt, va_list ap) {
    return redisAppendCommand(m_context.get(), fmt, ap);
}

int Redis::appendCmd(const std::vector<std::string> &argv) {
    std::vector<const char *> v;
    std::vector<size_t> l;
    for (auto &arg : argv) {
        v.push_back(arg.c_str());
        l.push_back(arg.length());
    }
    return redisAppendCommandArgv(m_context.get(), argv.size(), &v[0], &l[0]);
}

ReplyRpr Redis::getReply() {
    redisReply *r = nullptr;
    if (redisGetReply(m_context.get(), (void **)&r) == REDIS_OK) {
        ReplyRpr rt(r, freeReplyObject);
        return rt;
    }
    return nullptr;
}

RedisManager::RedisManager() { init(); }

IRedis::ptr RedisManager::get(const std::string &name) {
    RWMutexType::WRLock rlock(m_mutex);
    auto itr = m_datas.find(name);
    if (itr == m_datas.end() || itr->second.empty()) {
        return nullptr;
    }
    auto r = itr->second.front();
    itr->second.pop_front();
    rlock.unlock();

    return std::shared_ptr<IRedis>(
        r, std::bind(&RedisManager::freeRedis, this, std::placeholders::_1));
}

void RedisManager::freeRedis(IRedis *redis) {
    RWMutexType::WRLock rlock(m_mutex);
    m_datas[redis->getName()].push_back(redis);
}

void RedisManager::init() {
    m_config = g_redis->getVal();
    for (auto &i : m_config) {
        auto type = get_value(i.second, "type", "redis");
        auto pool = atoi(get_value(i.second, "pool", "1").c_str());
        while (pool--) {
            if (type == "redis") {
                Redis *rds = new Redis(i.first, i.second);
                rds->connect();
                RWMutexType::WRLock rlock(m_mutex);
                m_datas[i.first].push_back(rds);
            }
        }
    }
}

ReplyRpr RedisUtil::Cmd(const std::string &name, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    ReplyRpr rt = Cmd(name, fmt, ap);
    va_end(ap);
    return rt;
}

ReplyRpr RedisUtil::Cmd(const std::string &name, const char *fmt, va_list ap) {
    auto redis = RedisMgr::GetInstance()->get(name);
    if (!redis) {
        LYON_LOG_INFO(g_logger)
            << "Don't have any redis connection of : " << name;
        return nullptr;
    }
    return redis->cmd(fmt, ap);
}

ReplyRpr RedisUtil::Cmd(const std::string &name,
                        const std::vector<std::string> &args) {
    auto redis = RedisMgr::GetInstance()->get(name);
    if (!redis) {
        return nullptr;
    }
    return redis->cmd(args);
}

ReplyRpr RedisUtil::TryCmd(const std::string &name, uint32_t count,
                           const char *fmt, ...) {
    for (uint32_t i = 0; i < count; i++) {
        va_list ap;
        va_start(ap, fmt);
        ReplyRpr rt = Cmd(name, fmt, ap);
        va_end(ap);
        if (rt) {
            return rt;
        }
    }
    return nullptr;
}

ReplyRpr RedisUtil::TryCmd(const std::string &name, uint32_t count,
                           const std::vector<std::string> &args) {
    for (uint32_t i = 0; i < count; i++) {
        ReplyRpr rt = Cmd(name, args);
        if (rt) {
            return rt;
        }
    }
    return nullptr;
}

} // namespace lyon::db
