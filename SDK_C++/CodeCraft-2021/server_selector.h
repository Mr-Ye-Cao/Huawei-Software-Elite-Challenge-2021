// @Author: Yu Xin

#pragma once

#include <vector>
#include <unordered_map>

#include "vm_data_manager.h"
#include "vm_manager.h"
#include "server_data_manager.h"

class ServerSelector {
  public:
    // Meyer singleton
    static ServerSelector& GetInstance();
    ServerSelector(const ServerSelector&) = delete;
    ServerSelector& operator=(const ServerSelector&) = delete;
    void MakeServerSelection();
    std::pair<int16_t,int16_t> WorseCaseSelectionVm(const uint16_t& id, const uint16_t& worst_num);

  private:
    ServerSelector();
    ~ServerSelector() = default;
    ServerDataManager& server_data_manager_;
    VmDataManager& vm_data_manager_;
    VmManager& vm_manager_;
    std::vector<ServerInfo> selected_servers_;
    void MakeServerSelectionHelper(uint16_t curr_server_id, std::vector<uint16_t> server_list);
};