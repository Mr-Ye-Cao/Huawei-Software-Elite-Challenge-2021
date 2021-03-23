// @Author: Yu Xin

#pragma once

#include <vector>

#include "output_writer.h"
#include "request_data_manager.h"
#include "server_data_manager.h"
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
    std::vector<uint32_t> num_newly_added_tasks;
    std::vector<std::vector<bool> > vm_schedule_list; // schedule of each vm; the size of this list is the number of worst case vm.

    // Day, list of server id; currently unused, but might need it later when mapping VM's to servers
    // std::unordered_map<int32_t, std::vector<int32_t> > server_id_list;
};

class VmManager {
  public:
    // Meyer singleton
    static VmManager& GetInstance();
    VmManager(const VmManager&) = delete;
    VmManager& operator=(const VmManager&) = delete;
    
    std::unordered_map<uint16_t,uint16_t> vm_to_server_;

    std::unordered_map<uint16_t, VmStatusWorstCaseInfo>& GetWorstCaseVmList();
    std::pair<uint16_t, ServerInfo&> GetServerLambdaMatch(float lambda, bool fresh_start = false);
    void OutputTodayDeployment(const uint16_t& day);

  private:
    VmManager();
    ~VmManager() = default;

    VmDataManager& vm_data_manager_;
    RequestDataManager& request_data_manager_;
    ServerDataManager& server_data_manager_;
    OutputWriter& output_writer_;
    uint16_t days_; // Number of days (T)
    uint16_t num_vm_;
    std::unordered_map<int32_t, RequestInfo>& request_info_list_;
    
    // vm id (which is its index in vm_info_list_ in vm_data_manager_) to a list of schedules of all vm's of this kind
    // This assumes unlimited migration to give an optimistic estimation
    std::unordered_map<uint16_t, VmStatusInfo> vm_schedules_;

    // vm id (which is its index in vm_info_list_ in vm_data_manager_) to a list of schedules of all vm's of this kind
    // This assumes no migration to give an absolute worst case
    std::unordered_map<uint16_t, VmStatusWorstCaseInfo> vm_schedules_worst_case_;
    int64_t worst_case_core_;
    int64_t worst_case_memory_;
    std::vector<uint16_t> index_server_lambda_;
    int16_t prev_lambda_match_;
};
