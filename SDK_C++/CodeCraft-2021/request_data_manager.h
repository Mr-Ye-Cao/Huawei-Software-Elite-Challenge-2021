// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"

class RequestDataManager {
  public:
    RequestDataManager();
    ~RequestDataManager() = default;
  private:
    InputReader& input_reader_;
    std::vector<DailyRequestInfo>& daily_request_info_list_;
    
};
