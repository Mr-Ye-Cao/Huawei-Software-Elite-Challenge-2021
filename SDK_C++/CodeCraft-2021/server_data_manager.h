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
    
    // ALL ranking are done from smallest to largest
    ServerInfo& GetServerNthPurchaseCost(uint16_t n);
    ServerInfo& GetServerNthPurchaseCostCpu(uint16_t n); // Find nth best by the ratio of purchase cost : price
    ServerInfo& GetServerNthCpu(uint16_t n); // Gets the server with the nth smallest cpu
    ServerInfo& GetServerNthMemory(uint16_t n); // Gets the server with the nth smallest memory
    inline uint16_t GetNumServers() {
        return num_servers_;
    }

  private:
    ServerDataManager();
    ~ServerDataManager() = default;
    InputReader& input_reader_;
    std::vector<ServerInfo>& server_info_list_;
    std::vector<uint16_t> index_purchase_cost_;
    std::vector<uint16_t> index_purchase_cost_cpu_; // By the ratio of purchase cost : price 
    std::vector<uint16_t> index_cpu_;
    std::vector<uint16_t> index_memory_;
    uint16_t num_servers_;
    
    // le is less than or equal to, used as comparator
    void BuildIndexPurchaseCost(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/);
    void BuildIndexPurchaseCostCpu(); // Build index by the ratio of purchase cost : price 
    void BuildIndexCpu(); // Build index by the number of cpu's 
    void BuildIndexMemory(); // Build index by the size of memory 
};
