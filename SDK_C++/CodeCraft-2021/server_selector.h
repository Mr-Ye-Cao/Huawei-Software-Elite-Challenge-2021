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
    void MakeServerSelectionHelper(uint16_t curr_server_id, std::vector<uint16_t> server_list);
};
