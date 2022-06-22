#include "daemon.h"
#include <sys/wait.h>
#include <unistd.h>
namespace lyon {

static int real_start(int argc, char **argv,
                      std::function<int(int, char **)> main_cb) {
    ProcessInfoMgr::GetInstance()->main_pid = getpid();
    ProcessInfoMgr::GetInstance()->main_start_time = time(0);
    return main_cb(argc, argv);
}

static int real_daemon(int argc, char **argv,
                       std::function<int(int, char **)> main_cb) {
    daemon(1, 0);
    ProcessInfoMgr::GetInstance()->parent_pid = getpid();
    ProcessInfoMgr::GetInstance()->parent_start_time = time(0);
    while (true) {
        pid_t pid = fork();
        if (pid == 0) { //子进程返回
            ProcessInfoMgr::GetInstance()->main_pid = getpid();
            ProcessInfoMgr::GetInstance()->main_start_time = time(0);
            return real_start(argc, argv, main_cb);

        } else if (pid < 0) { //错误
            return -1;

        } else { //父进程返回
            int status = 0;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}

int start_daemon(int argc, char **argv,
                 std::function<int(int, char **)> main_cb, bool is_daemon) {
    if (is_daemon) {
        return real_daemon(argc, argv, main_cb);
    }
    ProcessInfoMgr::GetInstance()->parent_pid = getpid();
    ProcessInfoMgr::GetInstance()->parent_start_time = time(0);
    return real_start(argc, argv, main_cb);
}

} // namespace lyon
