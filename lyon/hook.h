#ifndef __LYON_HOOK_H__
#define __LYON_HOOK_H__
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

namespace lyon {
bool is_hook_enable();
void set_hook_enable(bool flag);

extern "C" {
// sleep

//这是声明一个函数类型sleep_fun
typedef unsigned int (*sleep_fun)(unsigned int seconds);
//这是声明一个该类型的变量，sleep_f
extern sleep_fun sleep_f;

typedef int (*usleep_fun)(useconds_t usec);
extern usleep_fun usleep_f;

// socket
typedef int (*socket_fun)(int domain, int type, int protocol);
extern socket_fun socket_f;

typedef int (*connect_fun)(int socket, const struct sockaddr *address,
                           socklen_t address_len);
extern connect_fun connect_f;

typedef int (*accept_fun)(int socket, struct sockaddr *address,
                          socklen_t *address_len);
extern accept_fun accept_f;

// read
typedef ssize_t (*read_fun)(int fildes, void *buf, size_t nbyte);
extern read_fun read_f;

typedef ssize_t (*readv_fun)(int fildes, const struct iovec *iov, int iovcnt);
extern readv_fun readv_f;

typedef ssize_t (*recv_fun)(int socket, void *buffer, size_t length, int flags);
extern recv_fun recv_f;

typedef ssize_t (*recvfrom_fun)(int socket, void *buffer, size_t length,
                                int flags, struct sockaddr *address,
                                socklen_t *address_len);
extern recvfrom_fun recvfrom_f;

typedef ssize_t (*recvmsg_fun)(int socket, struct msghdr *message, int flags);
extern recvmsg_fun recvmsg_f;

// write
typedef ssize_t (*write_fun)(int fildes, const void *buf, size_t nbyte);
extern write_fun write_f;

typedef ssize_t (*writev_fun)(int fildes, const struct iovec *iov, int iovcnt);
extern writev_fun writev_f;

typedef ssize_t (*send_fun)(int socket, const void *buffer, size_t length,
                            int flags);
extern send_fun send_f;

typedef ssize_t (*sendto_fun)(int socket, const void *message, size_t length,
                              int flags, const struct sockaddr *dest_addr,
                              socklen_t dest_len);
extern sendto_fun sendto_f;

typedef ssize_t (*sendmsg_fun)(int socket, const struct msghdr *message,
                               int flags);
extern sendmsg_fun sendmsg_f;

typedef int (*close_fun)(int fildes);
extern close_fun close_f;

// control
typedef int (*fcntl_fun)(int fildes, int cmd, ...);
extern fcntl_fun fcntl_f;

typedef int (*ioctl_fun)(int fildes, int request, ... /* arg */);
extern ioctl_fun ioctl_f;

typedef int (*getsockopt_fun)(int socket, int level, int option_name,
                              void *option_value, socklen_t *option_len);
extern getsockopt_fun getsockopt_f;

typedef int (*setsockopt_fun)(int socket, int level, int option_name,
                              const void *option_value, socklen_t option_len);
extern setsockopt_fun setsockopt_f;
}

} // namespace lyon
#endif // !__LYON_HOOK_H__
