#include "socket_stream.h"
namespace lyon {

int SocketStream::read(void *buffer, size_t size) {
    if (isConnected())
        return m_socket->recv(buffer, size);
    return -1;
}

int SocketStream::read(ByteArray::ptr bytearray, size_t size) {
    if (!isConnected()) {
        return -1;
    }
    std::vector<iovec> buffers;
    bytearray->getWriteBuffer(buffers, size);
    int rt = m_socket->recv(&buffers[0], buffers.size());
    if (rt > 0) {
        bytearray->setPosition(bytearray->getPosition() + rt);
    }
    return rt;
}

int SocketStream::write(void *buffer, size_t size) {
    if (isConnected()) {
        return m_socket->send(buffer, size);
    }
    return -1;
}

int SocketStream::write(ByteArray::ptr bytearray, size_t size) {
    if (!isConnected()) {
        return -1;
    }
    std::vector<iovec> buffers;
    bytearray->getReadBuffer(buffers, size);
    int rt = m_socket->send(&buffers[0], buffers.size());
    if (rt > 0) {
        bytearray->setPosition(bytearray->getPosition() + rt);
    }
    return rt;
}

} // namespace lyon
