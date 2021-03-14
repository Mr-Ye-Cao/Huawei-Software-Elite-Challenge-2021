// @Author: Yu Xin

#include "vm_manager.h"

VmManager::VmManager() :
  vm_data_manager_(VmDataManager::GetInstance()),
  request_data_manager_(RequestDataManager::GetInstance()),
  days_(request_data_manager_.GetDays()),
  num_vm_(vm_data_manager_.GetNumVm()),
  request_info_list_(request_data_manager_.GetDailyRequestInfoList()) {
    vm_schedules_.resize(num_vm_);
    for (uint16_t day = 0; day < days_; ++day) {
        
        // for (const auto& request : daily_request_info.request_info_list) {
        //     request.
        // }
    }
}
