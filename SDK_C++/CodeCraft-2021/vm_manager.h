// @Author: Yu Xin

#pragma once

#include <vector>

#include "output_writer.h"
#include "request_data_manager.h"
#include "server_data_manager.h"
// #include "server_manager.h"
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

struct VmToday {
    bool is_running;
    int32_t request_id;
};


// Status of this type of vm for calc worst case, including how many of this type of vm is running on each day
// and the schedules of these vm's
struct VmStatusWorstCaseInfo {
    std::unordered_map<int32_t, std::vector<VmToday> > vm_schedule_list; // unique id to schedule of each vm; the size of this map is the number of worst case vm.
    // std::unordered_map<int32_t, std::vector<int32_t> > request_id_list; // Day, list of server id
    // std::unordered_map<int32_t, int32_t> vm_unique_id_map; // Unique id to index in vm_schedule_list
    // Day, list of server id; currently unused, but might need it later when mapping VM's to servers
    // std::vector<uint16_t > server_id_list;
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
    int32_t GetRequestOnVm(uint16_t vm_id, int32_t vm_unique_id, uint16_t day); // Gets the request on this vm at the day
    std::vector<std::pair<uint16_t, int32_t> > GetNewVmOfToday(uint16_t day); // Gets the newly added vm's of the day (vm_id, vm_unique_id)

  private:
    VmManager();
    ~VmManager() = default;

    VmDataManager& vm_data_manager_;
    RequestDataManager& request_data_manager_;
    ServerDataManager& server_data_manager_;
    // ServerManager& server_manager_;
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
    // std::vector<uint32_t> num_newly_added_tasks_; // number of newly added tasks of each day
    int64_t worst_case_core_;
    int64_t worst_case_memory_;
    std::vector<uint16_t> index_server_lambda_;
    int16_t prev_lambda_match_;
    int32_t curr_unused_vm_id_ = 0;
};
