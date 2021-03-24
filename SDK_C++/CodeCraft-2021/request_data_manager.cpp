// @Author: Yu Xin

#include "request_data_manager.h"

// TODO(Yu Xin): to be implemented.
RequestDataManager::RequestDataManager() :
  input_reader_(InputReader::GetInstance()),
  days_(input_reader_.GetT()),
  request_info_list_(input_reader_.GetRequestInfoList()) {
    requests_by_start_day_.resize(days_);
    for (const auto& request : request_info_list_) {
        requests_by_start_day_[request.second.start_day].push_back(request.first);
    }
}

RequestDataManager& RequestDataManager::GetInstance() {
    static RequestDataManager request_data_manager;
    return request_data_manager;
}

uint16_t RequestDataManager::GetDays() {
    return days_;
}

uint16_t RequestDataManager::GetVmId(int32_t request_id) {
    return request_info_list_[request_id].vm_id;
}

int32_t RequestDataManager::GetUniqueVmId(int32_t request_id) {
    return request_info_list_[request_id].unique_vm_id;
}

std::vector<int32_t>& RequestDataManager::GetRequestOfDay(uint16_t day) {
    return requests_by_start_day_[day];
}

std::unordered_map<int32_t, RequestInfo>& RequestDataManager::GetRequestInfoList() {
    return request_info_list_;
}
