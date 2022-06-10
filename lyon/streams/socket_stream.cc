#include "socket_stream.h"
#include <lyon/log.h>
namespace lyon {

static Logger::ptr g_logger = LYON_LOG_GET_LOGGER("system");

ssize_t SocketStream::read(void *buffer, size_t size) {
    if (isConnected())
        return m_socket->recv(buffer, size);
    return -1;
}

ssize_t SocketStream::read(ByteArray::ptr bytearray, size_t size) {
    if (!isConnected()) {
        return -1;
    }
    std::vector<iovec> buffers;
    bytearray->getWriteBuffer(buffers, size);
    // LYON_LOG_DEBUG(g_logger) << "start sock read";
    ssize_t rt = m_socket->recv(&buffers[0], buffers.size());
    // LYON_LOG_DEBUG(g_logger) << "sock read rt = " << rt;
    if (rt > 0) {
        bytearray->setPosition(bytearray->getPosition() + rt);
    }
    return rt;
}

ssize_t SocketStream::write(const void *buffer, size_t size) {
    if (isConnected()) {
        return m_socket->send(buffer, size);
    }
    return -1;
}

ssize_t SocketStream::write(ByteArray::ptr bytearray, size_t size) {
    if (!isConnected()) {
        return -1;
    }
    std::vector<iovec> buffers;
    bytearray->getReadBuffer(buffers, size);
    ssize_t rt = m_socket->send(&buffers[0], buffers.size());
    if (rt > 0) {
        bytearray->setPosition(bytearray->getPosition() + rt);
    }
    return rt;
}

void SocketStream::close() {
    if (m_socket && m_socket->isConnected())
        m_socket->close();
}

SocketStream::~SocketStream() {
    if (m_socket && m_socket->isConnected())
        m_socket->close();
}

} // namespace lyon
