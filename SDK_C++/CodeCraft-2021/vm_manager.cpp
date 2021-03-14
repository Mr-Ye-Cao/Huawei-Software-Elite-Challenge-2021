// @Author: Yu Xin

#include <iostream>

#include "vm_manager.h"

VmManager::VmManager() :
  vm_data_manager_(VmDataManager::GetInstance()),
  request_data_manager_(RequestDataManager::GetInstance()),
  days_(request_data_manager_.GetDays()),
  num_vm_(vm_data_manager_.GetNumVm()),
  request_info_list_(request_data_manager_.GetDailyRequestInfoList()) {
    vm_schedules_.resize(num_vm_);
    for (const auto& request : request_info_list_) {
        const int16_t request_id = request.first;
        const RequestInfo& request_info = request.second;
        int vm_id = vm_data_manager_.GetVmId(request_info.requested_vm_name);
        std::cout << "Requested vm name is " << request_info.requested_vm_name << std::endl;
        std::cout << "Requested vm id is " << vm_id << std::endl;
    }
}

VmManager& VmManager::GetInstance() {
    static VmManager vm_manager;
    return vm_manager;
}
