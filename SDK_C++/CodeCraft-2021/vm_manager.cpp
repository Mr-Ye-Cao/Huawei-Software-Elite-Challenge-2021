// @Author: Yu Xin

#include <iostream>

#include "vm_manager.h"

VmManager::VmManager() :
  vm_data_manager_(VmDataManager::GetInstance()),
  request_data_manager_(RequestDataManager::GetInstance()),
  days_(request_data_manager_.GetDays()),
  num_vm_(vm_data_manager_.GetNumVm()),
  request_info_list_(request_data_manager_.GetRequestInfoList()) {
    vm_schedules_.reserve(num_vm_);
    for (const auto& request : request_info_list_) {
        const int32_t request_id = request.first;
        const RequestInfo& request_info = request.second;
        int vm_id = vm_data_manager_.GetVmId(request_info.requested_vm_name);

        // std::cout << "Request " << request_id << " needs vm " << vm_id << " from day " << request_info.start_day << " to day " << request_info.end_day << std::endl;

        VmStatusInfo& curr_vm_status = vm_schedules_[vm_id];
        curr_vm_status.num_running.resize(days_);
        for (int16_t day = request_info.start_day; day <= request_info.end_day; ++day) {
            ++curr_vm_status.num_running[day];
            curr_vm_status.request_id_list[day].push_back(request_id);
            // std::cout << "Request " << curr_vm_status.request_id_list[day].back()
            //     << " added, we have " << curr_vm_status.num_running[day] << " "
            //     << request_info.requested_vm_name << " running on day " << day << std::endl;
        }
    }
}

VmManager& VmManager::GetInstance() {
    static VmManager vm_manager;
    return vm_manager;
}
