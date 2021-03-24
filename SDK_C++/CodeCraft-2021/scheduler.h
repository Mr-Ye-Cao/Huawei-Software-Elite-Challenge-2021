// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"
#include "output_writer.h"
#include "request_data_manager.h"
#include "server_selector.h"
#include "vm_manager.h"

class Scheduler {
  public:
    // Meyer singleton
    static Scheduler& GetInstance();
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    void Run();

  private:
    Scheduler();
    ~Scheduler() = default;
    void OneIterationInternal();

    RequestDataManager& request_data_manager_;
    ServerSelector& server_selector_;
    VmManager& vm_manager_;
    OutputWriter& output_writer_;

    int32_t num_new_migrations_ = 0;
    uint16_t today_ = 0;
};
