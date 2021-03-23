// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"

class RequestDataManager {
  public:
    // Meyer singleton
    static RequestDataManager& GetInstance();
    RequestDataManager(const RequestDataManager&) = delete;
    RequestDataManager& operator=(const RequestDataManager&) = delete;

    uint16_t GetDays();
    std::vector<int32_t>& GetRequestOfDay(uint16_t day);
    std::unordered_map<int32_t, RequestInfo>& GetRequestInfoList();

  private:
    RequestDataManager();
    ~RequestDataManager() = default;

    InputReader& input_reader_;
    uint16_t days_;
    std::vector<std::vector<int32_t> > requests_by_start_day_;
    std::unordered_map<int32_t, RequestInfo>& request_info_list_;
    
};
