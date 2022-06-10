#ifndef __LYON_STREAMS_SOCKET_STREAM_H__
#define __LYON_STREAMS_SOCKET_STREAM_H__
#include "lyon/socket.h"
#include "stream.h"

namespace lyon {

/**
 * @brief Socket流的封装类
 * 封装了从socket中读取和写入数据的操作
 */
class SocketStream : public Stream {
public:
    typedef std::shared_ptr<SocketStream> ptr;

    SocketStream(Socket::ptr sock) : m_socket(sock) {}

    ~SocketStream();

    virtual ssize_t read(void *buffer, size_t size) override;

    virtual ssize_t read(ByteArray::ptr bytearray, size_t size) override;

    virtual ssize_t write(const void *buffer, size_t size) override;

    virtual ssize_t write(ByteArray::ptr bytearray, size_t size) override;

    bool isConnected() const { return m_socket && m_socket->isConnected(); };

    Socket::ptr getSocket() const { return m_socket; }

    virtual void close() override;

private:
    Socket::ptr m_socket;
};
} // namespace lyon

#endif
