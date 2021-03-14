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
    std::unordered_map<int16_t, RequestInfo>& GetDailyRequestInfoList();

  private:
    RequestDataManager();
    ~RequestDataManager() = default;

    InputReader& input_reader_;
    uint16_t days_;
    std::unordered_map<int16_t, RequestInfo>& request_info_list_;

    
};
