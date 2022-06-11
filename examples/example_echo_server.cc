#include "lyon/tcp_server.h"
#include <lyon/address.h>
#include <lyon/iomanager.h>
#include <lyon/log.h>
#include <lyon/serialize/bytearray.h>
#include <lyon/socket.h>
#include <memory>
#include <sys/socket.h>
#include <vector>

static lyon::Logger::ptr g_logger = LYON_LOG_GET_ROOT();

class EchoServer : public lyon::TcpServer {
public:
    typedef std::shared_ptr<EchoServer> ptr;

    EchoServer() { lyon::TcpServer(); }

    virtual void handleClient(lyon::Socket::ptr sock) override {
        lyon::ByteArray::ptr ba(new lyon::ByteArray());

        while (true) {
            std::vector<iovec> buffers;
            ba->clear();
            ba->getWriteBuffer(buffers, 1024);

            ssize_t rt = sock->recv(&buffers[0], buffers.size());

            if (rt == 0) {
                LYON_LOG_INFO(g_logger) << "Client closed";
                break;
            } else if (rt < 0) {
                LYON_LOG_INFO(g_logger)
                    << "Client error error = " << strerror(errno);
                break;
            }

            ba->setPosition(ba->getPosition() + rt);
            LYON_LOG_INFO(g_logger) << "Received message = " << ba->toString(0);
        }
    }
};

void run() {
    lyon::Address::ptr addr =
        lyon::Address::LookUpAnyIpAddress("0.0.0.0:8088", AF_INET, SOCK_STREAM);

    EchoServer::ptr server(new EchoServer());
    server->bindAndListen(addr);

    server->start();
}

int main(int argc, char *argv[]) {
    lyon::IOManager iom(2);
    iom.addJob(run);
    return 0;
}
