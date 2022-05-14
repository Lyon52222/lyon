#ifndef __LYON_TIMER_H__
#define __LYON_TIMER_H__

#include "mutex.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <set>
#include <vector>
namespace lyon {

class TimerManager;

class Timer : public std::enable_shared_from_this<Timer> {
    friend TimerManager;

public:
    typedef std::shared_ptr<Timer> ptr;

    bool cancle();
    bool refresh();
    bool reset(uint64_t ms, std::function<void()> cb, bool cycle);
    bool reset(uint64_t ms, bool from_now);

private:
    Timer(uint64_t ms, std::function<void()> cb, bool cycle,
          TimerManager *manager);

    Timer(uint64_t next);

    struct Comparator {
        bool operator()(const Timer::ptr &lhs, const Timer::ptr &rhs);
    };

private:
    uint64_t m_ms = 0;
    uint64_t m_next = 0;
    bool m_cycle = false;
    std::function<void()> m_cb = nullptr;
    TimerManager *m_manager;
};

class TimerManager {
    friend Timer;
    typedef RWMutex RWMutexType;

public:
    Timer::ptr addTimer(uint64_t ms, std::function<void()> cb,
                        bool cycle = false);

    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb,
                                 std::weak_ptr<void> cond, bool cycle = false);

    /**
     * @brief 获取距离下一个定时器执行还有多少毫秒
     *
     * @return 距离下一个定时器执行的时间
     */
    uint64_t getNextTimer();

    void listExpiredCbs(std::vector<std::function<void()>> &cbs);

protected:
    void addTimer(Timer::ptr val);
    void addTimer(Timer::ptr val, RWMutexType::WRLock &wlock);
    virtual void onTimerInsertAtFront() = 0;

private:
private:
    RWMutexType m_mutex;
    std::set<Timer::ptr, Timer::Comparator> m_timers;
};

} // namespace lyon
#endif
