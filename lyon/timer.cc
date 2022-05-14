#include "timer.h"
#include "mutex.h"
#include "util.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace lyon {

Timer::Timer(uint64_t ms, std::function<void()> cb, bool cycle,
             TimerManager *manager)
    : m_ms(ms), m_cycle(cycle), m_cb(cb), m_manager(manager) {
    m_next = GetCurrentTimeMS() + m_ms;
}

Timer::Timer(uint64_t next) : m_next(next) {}

bool Timer::Comparator::operator()(const Timer::ptr &lhs,
                                   const Timer::ptr &rhs) {
    if (!lhs)
        return true;
    if (!rhs)
        return false;
    return lhs->m_next != rhs->m_next ? lhs->m_next < rhs->m_next
                                      : lhs.get() < rhs.get();
}

bool Timer::cancle() {
    if (m_cb) {
        m_cb = nullptr;
        TimerManager::RWMutexType::WRLock wlock(m_manager->m_mutex);
        m_manager->m_timers.erase(shared_from_this());
        return true;
    }
    return false;
}

bool Timer::refresh() {
    if (m_cb) {
        TimerManager::RWMutexType::WRLock wlock(m_manager->m_mutex);
        m_manager->m_timers.erase(shared_from_this());
        m_next = GetCurrentTimeMS() + m_ms;
        m_manager->m_timers.insert(shared_from_this());
        return true;
    }
    return false;
}

bool Timer::reset(uint64_t ms, std::function<void()> cb, bool cycle) {
    TimerManager::RWMutexType::WRLock wlock(m_manager->m_mutex);
    m_manager->m_timers.erase(shared_from_this());
    m_next = GetCurrentTimeMS() + m_ms;
    m_cb = cb;
    m_cycle = cycle;
    m_manager->m_timers.insert(shared_from_this());
    return true;
}

bool Timer::reset(uint64_t ms, bool from_now) {
    TimerManager::RWMutexType::WRLock wlock(m_manager->m_mutex);
    m_manager->m_timers.erase(shared_from_this());
    if (from_now) {
        m_next = GetCurrentTimeMS() + m_ms;
    } else {
        m_next = m_next - m_ms + ms;
    }
    m_ms = ms;
    m_manager->addTimer(shared_from_this(), wlock);
    return true;
}

Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void()> cb,
                                  bool cycle) {
    Timer::ptr timer(new Timer(ms, cb, cycle, this));
    addTimer(timer);
    return timer;
}

static void CondWrapper(std::weak_ptr<void> cond, std::function<void()> cb) {
    if (!cond.expired()) {
        cb();
    }
}

Timer::ptr TimerManager::addConditionTimer(uint64_t ms,
                                           std::function<void()> cb,
                                           std::weak_ptr<void> cond,
                                           bool cycle) {
    return addTimer(ms, std::bind(CondWrapper, cond, cb), cycle);
}

void TimerManager::addTimer(Timer::ptr val) {
    RWMutexType::WRLock wlock(m_mutex);
    auto itr = m_timers.insert(val);
    bool insertatfront = (itr.first == m_timers.begin());
    wlock.unlock();
    if (insertatfront) {
        onTimerInsertAtFront();
    }
}

void TimerManager::addTimer(Timer::ptr val, RWMutexType::WRLock &wlock) {
    auto itr = m_timers.insert(val);
    bool insertatfront = (itr.first == m_timers.begin());
    wlock.unlock();
    if (insertatfront) {
        onTimerInsertAtFront();
    }
}
uint64_t TimerManager::getNextTimer() {
    RWMutexType::RDLock rlock(m_mutex);
    if (m_timers.empty()) {
        return ~0ull;
    }

    const Timer::ptr &next = *m_timers.begin();
    uint64_t now_ms = GetCurrentTimeMS();
    if (next->m_next <= now_ms) {
        return 0;
    } else {
        return next->m_next - now_ms;
    }
}

void TimerManager::listExpiredCbs(std::vector<std::function<void()>> &cbs) {
    uint64_t now_ms = GetCurrentTimeMS();
    std::vector<Timer::ptr> expires;
    {
        RWMutexType::RDLock rlock(m_mutex);
        if (m_timers.empty())
            return;
    }
    RWMutexType::WRLock wlock(m_mutex);
    Timer::ptr now_timer(new Timer(now_ms));

    auto itr = m_timers.upper_bound(now_timer);
    while (itr != m_timers.end() && (*itr)->m_next == now_timer->m_next) {
        itr++;
    }

    expires.insert(expires.begin(), m_timers.begin(), itr);
    m_timers.erase(m_timers.begin(), itr);

    cbs.reserve(expires.size());

    for (auto &timer : expires) {
        cbs.push_back(timer->m_cb);
        if (timer->m_cycle) {
            timer->m_next = now_ms + timer->m_ms;
            m_timers.insert(timer);
        } else {
            timer->m_cb = nullptr;
        }
    }
}

} // namespace lyon
