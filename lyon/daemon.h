#ifndef __LYON_DAEMON_H__
#define __LYON_DAEMON_H__

#include <cstdint>
#include <functional>
#include <singleton.h>

namespace lyon {

struct ProcessInfo {
    pid_t parent_pid = 0;
    pid_t main_pid = 0;
    uint64_t parent_start_time = 0;
    uint64_t main_start_time = 0;
    uint32_t restart_count = 0;
};

typedef Singleton<ProcessInfo> ProcessInfoMgr;

int start_daemon(int argc, char **argvProcessInfo,
                 std::function<int(int, char **)> main_cb, bool is_daemon);
} // namespace lyon
#endif
