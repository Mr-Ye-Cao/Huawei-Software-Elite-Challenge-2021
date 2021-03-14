// @Author: Yu Xin

#include "server_data_manager.h"

#include <iostream>

#include "index_comparator.h"

// TODO(Yu Xin): to be implemented.
ServerDataManager::ServerDataManager() :
  input_reader_(InputReader::GetInstance()),
  server_info_list_(input_reader_.GetServerInfoList()) {
    std::cout << "Server manager constructed\n";
    index_purchase_cost_.resize(server_info_list_.size());
    for (uint16_t i = 0; i < server_info_list_.size(); ++i) {
        index_purchase_cost_[i] = i;
    }
    BuildIndexPurchaseCost();
}

void ServerDataManager::BuildIndexPurchaseCost(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/) {
    std::sort(
        index_purchase_cost_.begin(),
        index_purchase_cost_.end(),
        IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
            server_info_list_.begin(),
            server_info_list_.end(),
            [] (const ServerInfo& a, const ServerInfo& b) -> bool {
                return a.purchase_cost < b.purchase_cost;
        })
    );
    for (int i = 0; i < index_purchase_cost_.size(); ++i) {
        std::cout << "No. " << i << " is " << server_info_list_[index_purchase_cost_[i]].purchase_cost << std::endl;
    }
}

ServerInfo& ServerDataManager::GetServerNthPurchaseCost(uint16_t n) {
    return server_info_list_[index_purchase_cost_[n]];
}
