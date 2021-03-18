// @Author: Yu Xin

#pragma once

#include <vector>

#include "request_data_manager.h"
#include "vm_data_manager.h"

// The VM's schedule
// struct VmScheduleInfo {
//     std::vector<bool> is_running;
//     std::vector<int32_t> request_id;
//     std::vector<int32_t> server_id; // TODO(Yu Xin): currently unused, but might need it later
//                                     // when mapping VM's to servers
// };

// Status of this type of vm, including how many of this type of vm is running on each day
// and the schedules of these vm's
struct VmStatusInfo {
    std::vector<uint32_t> num_running;
    std::unordered_map<int32_t, std::vector<int32_t> > request_id_list; // Day, list of server id

    // Day, list of server id; currently unused, but might need it later when mapping VM's to servers
    std::unordered_map<int32_t, std::vector<int32_t> > server_id_list;
};

// Status of this type of vm for calc worst case, including how many of this type of vm is running on each day
// and the schedules of these vm's
struct VmStatusWorstCaseInfo {
    // std::vector<uint32_t> num_running;
    std::vector<std::vector<bool> > vm_schedule_list; // schedule of each vm

    // Day, list of server id; currently unused, but might need it later when mapping VM's to servers
    // std::unordered_map<int32_t, std::vector<int32_t> > server_id_list;
};

class VmManager {
  public:
    // Meyer singleton
    static VmManager& GetInstance();
    VmManager(const VmManager&) = delete;
    VmManager& operator=(const VmManager&) = delete;

  private:
    VmManager();
    ~VmManager() = default;

    VmDataManager& vm_data_manager_;
    RequestDataManager& request_data_manager_;
    uint16_t days_; // Number of days (T)
    uint16_t num_vm_;
    std::unordered_map<int32_t, RequestInfo>& request_info_list_;
    // vm id to a list of schedules of all vm's of this kind
    // TODO(Yu Xin): use next_available when adding requests to the list
    std::unordered_map<uint16_t, VmStatusInfo> vm_schedules_;
    std::unordered_map<uint16_t, VmStatusWorstCaseInfo> vm_schedules_worst_case_;
};
