#ifndef __LYON_DB_REDIS_H__
#define __LYON_DB_REDIS_H__

#include <cstdint>
#include <hiredis/hiredis.h>
#include <memory>
#include <vector>

namespace lyon::db {

typedef std::shared_ptr<redisReply> ReplyRpr;

class IRedis {
public:
    typedef std::shared_ptr<IRedis> ptr;

    IRedis(bool logEnable = true) : m_logEnable(logEnable) {}
    virtual ~IRedis();

    virtual ReplyRpr cmd(const char *fmt, ...) = 0;
    virtual ReplyRpr cmd(const char *fmt, va_list ap) = 0;
    virtual ReplyRpr cmd(const char *fmt,
                         const std::vector<std::string> &args) = 0;

    const std::string &getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    const std::string &getPassword() const { return m_password; }
    void setPassword(const std::string &password) { m_password = password; }

protected:
    bool m_logEnable;
    std::string m_name;
    std::string m_password;
};

class SyncIRedis : public IRedis {
public:
    typedef std::shared_ptr<SyncIRedis> ptr;

    virtual ~SyncIRedis();

    virtual bool connect() = 0;
    virtual bool connect(const std::string &host, uint16_t port,
                         uint64_t ms) = 0;
    virtual bool reconnect() = 0;

    virtual int appendCmd(const char *fmt, ...) = 0;
    virtual int appendCmd(const char *fmt, va_list ap) = 0;
    virtual int appendCmd(const char *fmt,
                          const std::vector<std::string> &args) = 0;

protected:
    uint64_t m_lastActiveTime;
    std::string m_host;
    uint16_t m_port;
};

class Redis : SyncIRedis {
public:
    typedef std::shared_ptr<Redis> ptr;
    virtual ~Redis();

    virtual bool connect() override;
    virtual bool connect(const std::string &host, uint16_t port,
                         uint64_t ms) override;
    virtual bool reconnect() override;

    virtual ReplyRpr cmd(const char *fmt, ...) override;
    virtual ReplyRpr cmd(const char *fmt, va_list ap) override;
    virtual ReplyRpr cmd(const char *fmt,
                         const std::vector<std::string> &args) override;

    virtual int appendCmd(const char *fmt, ...) override;
    virtual int appendCmd(const char *fmt, va_list ap) override;
    virtual int appendCmd(const char *fmt,
                          const std::vector<std::string> &args) override;

private:
    std::shared_ptr<redisContext> m_context;
};

} // namespace lyon::db
#endif
