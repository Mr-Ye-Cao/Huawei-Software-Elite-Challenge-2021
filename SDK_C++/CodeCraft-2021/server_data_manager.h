// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"

class ServerDataManager {
  public:
    ServerDataManager();
    ~ServerDataManager() = default;
  private:
    InputReader& input_reader_;
    std::vector<ServerInfo>& server_info_list_;
};
