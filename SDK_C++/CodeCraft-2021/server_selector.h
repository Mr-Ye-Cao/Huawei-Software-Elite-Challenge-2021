// @Author: Yu Xin

#pragma once

#include <vector>
#include <unordered_map>

#include "output_writer.h"
#include "server_data_manager.h"
#include "server_manager.h"
#include "vm_data_manager.h"
#include "vm_manager.h"

class ServerSelector {
  public:
    // Meyer singleton
    static ServerSelector& GetInstance();
    ServerSelector(const ServerSelector&) = delete;
    ServerSelector& operator=(const ServerSelector&) = delete;
    // void MakeServerSelection();
    void MakeServerSelectionSimple(uint16_t today);
    // std::unordered_map<std::uint16_t, std::uint16_t>& GetServerPurchaseChart();
    uint16_t GetNumNewPurchases();
    void ResetTodayPurchases();
    void OutputTodayServerPurchases();

  private:
    ServerSelector();
    ~ServerSelector() = default;
    ServerDataManager& server_data_manager_;
    VmDataManager& vm_data_manager_;
    VmManager& vm_manager_;
    ServerManager& server_manager_;
    OutputWriter& output_writer_;
    // std::vector<ServerInfo> selected_servers_;
    std::unordered_map<std::uint16_t, std::uint16_t> server_purchase_chart_; // static server id to number to buy
    std::unordered_map<std::uint16_t, std::uint16_t> server_purchase_chart_today_;
    uint16_t server_dynamic_id_ = 0;
    uint16_t total_server_num_ = 0;
    uint16_t num_new_purchases_ = 0;

  // Debug only
  uint16_t added_to_existing_server_ = 0;

    // std::pair<int16_t,int16_t> WorseCaseSelectionVm(const uint16_t& nth_smallest_lambda, const uint16_t& worst_num);
    // void MakeServerSelectionHelper(uint16_t curr_server_id, std::vector<uint16_t> server_list);
    // void PurchaseServers(uint16_t server_id, uint16_t num);
    uint16_t AddVmsToServers(uint16_t server_id, uint16_t vm_id, int32_t vm_unique_key);
    void ApplyTodayPurchase();
};