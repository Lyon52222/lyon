#ifndef __LYON_TCP_SERVER_H__
#define __LYON_TCP_SERVER_H__
#include "iomanager.h"
#include <memory>
namespace lyon {
class TcpServer {
public:
    typedef std::shared_ptr<TcpServer> ptr;

private:
    IOManager *m_worker;
    IOManager *m_ioworker;
    IOManager *m_acceptWorker;
};

} // namespace lyon

#endif
