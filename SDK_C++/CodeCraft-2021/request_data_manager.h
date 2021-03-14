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

  private:
    RequestDataManager();
    ~RequestDataManager() = default;
    InputReader& input_reader_;
    std::vector<DailyRequestInfo>& daily_request_info_list_;

    
};
