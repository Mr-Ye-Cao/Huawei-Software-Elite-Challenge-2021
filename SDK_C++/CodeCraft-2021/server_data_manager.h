// @Author: Yu Xin

#pragma once

#include <queue>
#include <vector>

#include "input_reader.h"

namespace {
  const uint8_t kNumIntervals = 10; // has to be even number
} // namespace


struct ServerOfLambdaInterval {
  float lambda_start;
  float lambda_end;
  uint16_t server_id;
};

class ServerDataManager {
  public:
    // the number server
    std::vector<uint16_t> purchase_list_;

    // Meyer singleton
    static ServerDataManager& GetInstance();
    ServerDataManager(const ServerDataManager&) = delete;
    ServerDataManager& operator=(const ServerDataManager&) = delete;
    
    // ALL ranking are done from smallest to largest
    ServerInfo& GetServerInfo(uint16_t n);
    // std::pair<uint16_t, ServerInfo> GetServerNthBruteForce(uint16_t n);
    // ServerInfo& GetServerNthPurchaseCostCpu(uint16_t n); // Find nth best by the ratio of purchase cost : price
    // ServerInfo& GetServerNthCpu(uint16_t n); // Gets the server with the nth smallest cpu
    // ServerInfo& GetServerNthMemory(uint16_t n); // Gets the server with the nth smallest memory
    // std::pair<uint16_t, ServerInfo> GetServerLambdaMatch(float lambda, bool fresh_start = false);
    std::pair<uint16_t, ServerInfo> GetServerLambdaIntervalMatch(float lambda);
    // Gets the best server that fist this vm
    // uint16_t GetBestServerThatFits(int16_t vm_cpu, int16_t vm_memory, bool is_single);
    inline uint16_t GetNumServers() {
        return num_servers_;
    }

  private:
    ServerDataManager();
    ~ServerDataManager() = default;
    InputReader& input_reader_;
    std::vector<ServerInfo>& server_info_list_;
    // std::vector<uint16_t> index_brute_force_;
    // std::vector<uint16_t> index_purchase_cost_cpu_; // By the ratio of purchase cost : price 
    // std::vector<uint16_t> index_cpu_;
    // std::vector<uint16_t> index_memory_;
    std::vector<uint16_t> index_server_lambda_;
    uint16_t num_servers_;
    uint16_t prev_lambda_match_;
    std::vector<ServerOfLambdaInterval> server_lambda_match_;
    
    // le is less than or equal to, used as comparator
    // void BuildIndexBruteForce(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/);
    // void BuildIndexPurchaseCostCpu(); // Build index by the ratio of purchase cost : price 
    // void BuildIndexCpu(); // Build index by the number of cpu's 
    // void BuildIndexMemory(); // Build index by the size of memory 
    void BuildIndexServerLambda(); // Lambda is CPU / Memory 
    void BuildServerLambdaIntervalMatch();
};
