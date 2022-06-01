#ifndef __LYON_STREAMS_SOCKET_STREAM_H__
#define __LYON_STREAMS_SOCKET_STREAM_H__
#include "lyon/socket.h"
#include "lyon/stream.h"

namespace lyon {

class SocketStream : public Stream {
public:
    typedef std::shared_ptr<SocketStream> ptr;

    SocketStream(Socket::ptr sock) : m_socket(sock) {}

    virtual int read(void *buffer, size_t size) override;

    virtual int read(ByteArray::ptr bytearray, size_t size) override;

    virtual int write(const void *buffer, size_t size) override;

    virtual int write(ByteArray::ptr bytearray, size_t size) override;

    bool isConnected() const { return m_socket && m_socket->isConnected(); };

    void close();

    ~SocketStream();

private:
    Socket::ptr m_socket;
};
} // namespace lyon

#endif
