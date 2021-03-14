// @Author: Yu Xin

#pragma once

#include <queue>
#include <vector>

#include "input_reader.h"

class ServerDataManager {
  public:
    ServerDataManager();
    ~ServerDataManager() = default;
    ServerInfo& GetServerNthPurchaseCost(uint16_t n);

  private:
    InputReader& input_reader_;
    std::vector<ServerInfo>& server_info_list_;
    std::vector<uint16_t> index_purchase_cost_;
    // le is less than or equal to, used as comparator
    void BuildIndexPurchaseCost(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/);
};
