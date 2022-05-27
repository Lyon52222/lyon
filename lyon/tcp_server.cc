#include "tcp_server.h"
#include "config.h"
#include <cstdint>
#include <iomanager.h>
#include <log.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

static ConfigVar<uint64_t>::ptr g_tcp_recv_timeout =
    Config::SetConfig<uint64_t>("tcp.recv.timeout", 60 * 1000 * 2,
                                "tcp recv timeout");

static uint64_t s_tcp_recv_timeout = -1;

namespace {
struct _TcpServerIniter {
    _TcpServerIniter() {
        s_tcp_recv_timeout = g_tcp_recv_timeout->getVal();
        g_tcp_recv_timeout->addOnChange(
            [](const uint64_t &ov, const uint64_t &nv) {
                s_tcp_recv_timeout = g_tcp_recv_timeout->getVal();
            });
    }
};

static _TcpServerIniter tcp_server_initer;

} // namespace

TcpServer::TcpServer(IOManager *worker, IOManager *ioworker,
                     IOManager *accpetWorker)
    : m_worker(worker), m_ioworker(ioworker), m_acceptWorker(accpetWorker),
      m_recvTimeout(s_tcp_recv_timeout), m_name("lyon/1.0.0"), m_isStop(true) {}

TcpServer::~TcpServer() {
    for (auto &s : m_sockets) {
        s->close();
    }
    m_sockets.clear();
}

bool TcpServer::bind(const std::vector<Address::ptr> &addrs,
                     const std::vector<Address::ptr> &failed_addrs) {
    return true;
}

} // namespace lyon
