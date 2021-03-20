// @Author: Yu Xin

#pragma once

#include <vector>
#include <map>

#include "vm_data_manager.h"
#include "vm_manager.h"
#include "server_data_manager.h"

class ServerSelector {
  public:
    ServerSelector();
    ~ServerSelector();
    void MakeServerSelection();

  private:
    ServerDataManager& server_data_manager_;
    std::map<int,int> associative;
    VmDataManager& vm_data_manager_;
    VmManager& vm_manager_;
    std::vector<ServerInfo> selected_servers_;
    void MakeServerSelectionHelper(uint16_t curr_server_id, std::vector<uint16_t> server_list);
    int WorseCaseSelectionVm(const uint16_t& id, const uint16_t& worst_num);
};
