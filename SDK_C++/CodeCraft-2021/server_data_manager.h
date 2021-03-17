// @Author: Yu Xin

#pragma once

#include <queue>
#include <vector>

#include "input_reader.h"

class ServerDataManager {
  public:
    // Meyer singleton
    static ServerDataManager& GetInstance();
    ServerDataManager(const ServerDataManager&) = delete;
    ServerDataManager& operator=(const ServerDataManager&) = delete;
    
    ServerInfo& GetServerNthPurchaseCost(uint16_t n);
    ServerInfo& GetServerNthPurchaseCostCore(uint16_t n);

  private:
    ServerDataManager();
    ~ServerDataManager() = default;
    InputReader& input_reader_;
    std::vector<ServerInfo>& server_info_list_;
    std::vector<uint16_t> index_purchase_cost_;
    std::vector<uint16_t> index_purchase_cost_core_; // By the ratio of purchase cost : price 
    // le is less than or equal to, used as comparator
    void BuildIndexPurchaseCost(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/);
    void BuildIndexPurchaseCostCore(); // Build index by the ratio of purchase cost : price 
};
