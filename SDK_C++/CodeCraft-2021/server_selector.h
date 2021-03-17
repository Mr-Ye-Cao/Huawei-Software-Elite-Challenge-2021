// @Author: Yu Xin

#pragma once

#include <vector>

#include "server_data_manager.h"

class ServerSelector {
  public:
    ServerSelector();
    ~ServerSelector();
    void MakeServerSelection();

  private:
    ServerDataManager& server_data_manager_;
    std::vector<ServerInfo> selected_servers_;
};
