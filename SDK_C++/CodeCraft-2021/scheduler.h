// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"

class Scheduler {
  public:
    // Meyer singleton
    static Scheduler& GetInstance();
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

  private:
    Scheduler();
    ~Scheduler() = default;

    
};
