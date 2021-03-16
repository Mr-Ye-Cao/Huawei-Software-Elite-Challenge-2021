// @Author: Yu Xin

#include "request_data_manager.h"

// TODO(Yu Xin): to be implemented.
RequestDataManager::RequestDataManager() :
  input_reader_(InputReader::GetInstance()),
  days_(input_reader_.GetT()),
  request_info_list_(input_reader_.GetRequestInfoList()) {}

RequestDataManager& RequestDataManager::GetInstance() {
    static RequestDataManager request_data_manager;
    return request_data_manager;
}

uint16_t RequestDataManager::GetDays() {
    return days_;
}

std::unordered_map<int32_t, RequestInfo>& RequestDataManager::GetRequestInfoList() {
    return request_info_list_;
}
