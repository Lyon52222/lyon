#include "tcp_server.h"
#include "address.h"
#include "config.h"
#include "iomanager.h"
#include "log.h"
#include "socket.h"
#include <cstdint>
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
                     IOManager *acceptWorker)
    : m_worker(worker), m_ioworker(ioworker), m_acceptWorker(acceptWorker),
      m_recvTimeout(s_tcp_recv_timeout), m_name("lyon/1.0.0"), m_isStop(true) {}

TcpServer::~TcpServer() {
    for (auto &s : m_sockets) {
        s->close();
    }
    m_sockets.clear();
}

bool TcpServer::bindAndListen(const Address::ptr addr, bool ssl) {
    std::vector<Address::ptr> addrs;
    std::vector<Address::ptr> failed_addrs;
    addrs.push_back(addr);
    return bindAndListen(addrs, failed_addrs, ssl);
}

bool TcpServer::bindAndListen(const std::vector<Address::ptr> &addrs,
                              std::vector<Address::ptr> &failed_addrs,
                              bool ssl) {
    for (auto &addr : addrs) {
        Socket::ptr sock = Socket::CreateTCP(addr);
        if (!sock->bind(addr)) {
            LYON_LOG_ERROR(g_logger)
                << "TcpServer::bindAndListen bind error. addr = " << addr;
            failed_addrs.push_back(addr);
            continue;
        }
        if (!sock->listen()) {
            LYON_LOG_ERROR(g_logger)
                << "TcpServer::bindAndListen listen error. addr = " << addr;
            failed_addrs.push_back(addr);
            continue;
        }
        m_sockets.push_back(sock);
    }
    if (!failed_addrs.empty()) {
        m_sockets.clear();
        return false;
    }

    for (auto &sock : m_sockets) {
        LYON_LOG_INFO(g_logger)
            << "TcpServer:" << m_name << "start listen on" << (*sock);
    }

    return true;
}

void TcpServer::startAccept(const Socket::ptr sock) {
    while (!m_isStop) {
        Socket::ptr nsock = sock->accept();
        if (nsock) {
            nsock->setRecvTimeout(s_tcp_recv_timeout);
            m_worker->addJob(
                std::bind(&TcpServer::handleClient, shared_from_this(), nsock));
        } else {
            LYON_LOG_ERROR(g_logger)
                << "TcpServer::startAccept accept error sock = " << *sock;
        }
    }
    return;
}

bool TcpServer::start() {
    if (!m_isStop) {
        return true;
    }
    m_isStop = false;
    for (auto &sock : m_sockets) {
        m_acceptWorker->addJob(
            std::bind(&TcpServer::startAccept, shared_from_this(), sock));
    }

    return true;
}

void TcpServer::stop() {
    m_isStop = true;
    m_acceptWorker->addJob([this]() {
        for (auto &sock : m_sockets) {
            sock->triggerAccept();
            sock->close();
        }
        m_sockets.clear();
    });
}

void TcpServer::handleClient(Socket::ptr sock) {
    LYON_LOG_INFO(g_logger) << "TcpServer::handleClient : " << *sock;
}

} // namespace lyon
