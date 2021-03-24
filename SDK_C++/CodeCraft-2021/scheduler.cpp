// @Author: Yu Xin

#include "scheduler.h"

// TODO(Yu Xin): to be implemented.
Scheduler::Scheduler() {
    
}

Scheduler& Scheduler::GetInstance() {
    static Scheduler scheduler;
    return scheduler;
}
