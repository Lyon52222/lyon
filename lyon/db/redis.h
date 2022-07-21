#ifndef __LYON_DB_REDIS_H__
#define __LYON_DB_REDIS_H__

#include "lyon/mutex.h"
#include "lyon/singleton.h"
#include <cstdint>
#include <hiredis/hiredis.h>
#include <list>
#include <map>
#include <memory>
#include <vector>

namespace lyon::db {

typedef std::shared_ptr<redisReply> ReplyRpr;

class IRedis {
public:
    typedef std::shared_ptr<IRedis> ptr;
    enum Type { REDIS = 1 };

    IRedis(bool logEnable = true) : m_logEnable(logEnable) {}
    virtual ~IRedis() {}

    virtual ReplyRpr cmd(const char *fmt, ...) = 0;
    virtual ReplyRpr cmd(const char *fmt, va_list ap) = 0;
    virtual ReplyRpr cmd(const std::vector<std::string> &argv) = 0;

    const std::string &getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    const std::string &getPassword() const { return m_password; }
    void setPassword(const std::string &password) { m_password = password; }

protected:
    bool m_logEnable;
    Type m_type;
    std::string m_name;
    std::string m_password;
};

class SyncIRedis : public IRedis {
public:
    typedef std::shared_ptr<SyncIRedis> ptr;

    virtual ~SyncIRedis() {}

    virtual bool connect() = 0;
    virtual bool connect(const std::string &host, uint16_t port,
                         uint64_t ms) = 0;
    virtual bool reconnect() = 0;

    virtual int appendCmd(const char *fmt, ...) = 0;
    virtual int appendCmd(const char *fmt, va_list ap) = 0;
    virtual int appendCmd(const std::vector<std::string> &argv) = 0;

    virtual ReplyRpr getReply() = 0;

protected:
    uint16_t m_port;
    uint64_t m_connectMs;
    uint64_t m_lastActiveTime;
    std::string m_host;
};

class Redis : public SyncIRedis {
public:
    typedef std::shared_ptr<Redis> ptr;
    Redis(const std::string &name);
    Redis(const std::string &name,
          const std::map<std::string, std::string> &conf);

    virtual ~Redis() {}

    virtual bool connect() override;
    virtual bool connect(const std::string &host, uint16_t port,
                         uint64_t ms) override;
    virtual bool reconnect() override;

    void setCmdTimeout(uint64_t to);

    virtual ReplyRpr cmd(const char *fmt, ...) override;
    virtual ReplyRpr cmd(const char *fmt, va_list ap) override;
    virtual ReplyRpr cmd(const std::vector<std::string> &argv) override;

    virtual int appendCmd(const char *fmt, ...) override;
    virtual int appendCmd(const char *fmt, va_list ap) override;
    virtual int appendCmd(const std::vector<std::string> &argv) override;

    virtual ReplyRpr getReply() override;

private:
    timeval m_cmdTimeout = {1, 0};
    std::shared_ptr<redisContext> m_context;
};

class RedisManager {
public:
    typedef std::shared_ptr<RedisManager> ptr;
    typedef RWMutex RWMutexType;

    RedisManager();
    IRedis::ptr get(const std::string &name);

private:
    void freeRedis(IRedis *redis);
    void init();

private:
    RWMutexType m_mutex;
    std::map<std::string, std::list<IRedis *>> m_datas;
    std::map<std::string, std::map<std::string, std::string>> m_config;
};

typedef Singleton<RedisManager> RedisMgr;

class RedisUtil {
public:
    static ReplyRpr Cmd(const std::string &name, const char *fmt, ...);
    static ReplyRpr Cmd(const std::string &name, const char *fmt, va_list ap);
    static ReplyRpr Cmd(const std::string &name,
                        const std::vector<std::string> &args);

    static ReplyRpr TryCmd(const std::string &name, uint32_t count,
                           const char *fmt, ...);
    static ReplyRpr TryCmd(const std::string &name, uint32_t count,
                           const std::vector<std::string> &args);
};

} // namespace lyon::db
#endif
