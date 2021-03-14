// @Author: Yu Xin

#pragma once

#include <vector>
#include <unordered_map>

#include "request_data_manager.h"
#include "vm_data_manager.h"

// The VM's schedule
struct VmScheduleInfo {
    uint32_t num_running;
    std::vector<bool> is_running;
    std::vector<int32_t> request_id;
    std::vector<int32_t> server_id; // TODO(Yu Xin): currently unused, but might need it later
                                    // when mapping VM's to servers
};

class VmManager {
  public:
    VmManager();
    ~VmManager() = default;

  private:
    VmDataManager& vm_data_manager_;
    RequestDataManager& request_data_manager_;
    // vm id to a list of schedules of all vm's of this kind
    // TODO(Yu Xin): use next_available when adding requests to the list
    std::unordered_map<uint16_t, std::vector<VmScheduleInfo> > vm_schedules_;
};
