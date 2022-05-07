#ifndef __LYON_SCHEDULER_H__
#define __LYON_SCHEDULER_H__

#include "fiber.h"
#include <memory>
namespace lyon {

/**
 * @brief 协程调度器，负责对协程进行调度
 */
class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;

public:
    static Fiber *GetMainFiber();
};

} // namespace lyon

#endif
