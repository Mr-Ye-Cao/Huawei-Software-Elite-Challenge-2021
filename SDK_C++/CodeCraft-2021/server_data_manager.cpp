// @Author: Yu Xin

#include "server_data_manager.h"

#include <algorithm>
#include <iostream>

#include "index_comparator.h"

ServerDataManager& ServerDataManager::GetInstance() {
    static ServerDataManager server_data_manager;
    return server_data_manager;
}

ServerDataManager::ServerDataManager() :
  input_reader_(InputReader::GetInstance()),
  server_info_list_(input_reader_.GetServerInfoList()) {
    // std::cout << "Server data manager constructed\n";
    
    purchase_list_.resize(server_info_list_.size());

    index_brute_force_.resize(server_info_list_.size());
    // index_purchase_cost_cpu_.resize(server_info_list_.size());
    index_cpu_.resize(server_info_list_.size());
    index_memory_.resize(server_info_list_.size());
    num_servers_ = server_info_list_.size();
    index_server_lambda_.resize(server_info_list_.size());
    for (uint16_t i = 0; i < num_servers_; ++i) {
        index_brute_force_[i] = i;
        // index_purchase_cost_cpu_[i] = i;
        index_cpu_[i] = i;
        index_memory_[i] = i;
        index_server_lambda_[i] = i;
    }
    BuildIndexBruteForce();
    // BuildIndexPurchaseCostCpu();
    BuildIndexCpu();
    BuildIndexMemory();
    BuildIndexServerLambda();
}

void ServerDataManager::BuildIndexBruteForce(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/) {
    std::sort(
        index_brute_force_.begin(),
        index_brute_force_.end(),
        IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
            server_info_list_.begin(),
            server_info_list_.end(),
            [] (const ServerInfo& a, const ServerInfo& b) -> bool {
                return a.purchase_cost < b.purchase_cost; // TODO: change this to your sorting criteria
        })
    );
    // for (int i = 0; i < index_brute_force_.size(); ++i) {
    //     std::cout << "No. " << i << " is " << server_info_list_[index_brute_force_[i]].purchase_cost << std::endl;
    // }
}

// void ServerDataManager::BuildIndexPurchaseCostCpu() {
//     std::sort(
//         index_purchase_cost_cpu_.begin(),
//         index_purchase_cost_cpu_.end(),
//         IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
//             server_info_list_.begin(),
//             server_info_list_.end(),
//             [] (const ServerInfo& a, const ServerInfo& b) -> bool {
//                 return (float)a.purchase_cost / a.server_memory < (float)b.purchase_cost / b.server_memory;
//         })
//     );
//     // for (int i = 0; i < index_purchase_cost_cpu_.size(); ++i) {
//     //     std::cout << "No. " << i << " costs " << server_info_list_[index_purchase_cost_cpu_[i]].purchase_cost << std::endl;
//     // }
// }

void ServerDataManager::BuildIndexCpu() {
    std::sort(
        index_cpu_.begin(),
        index_cpu_.end(),
        IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
            server_info_list_.begin(),
            server_info_list_.end(),
            [] (const ServerInfo& a, const ServerInfo& b) -> bool {
                return a.server_cpu < b.server_cpu;
        })
    );
    // for (int i = 0; i < index_cpu_.size(); ++i) {
    //     std::cout << "No. " << i << " cpu is " << server_info_list_[index_cpu_[i]].server_cpu << std::endl;
    // }
}

void ServerDataManager::BuildIndexMemory() {
    std::sort(
        index_memory_.begin(),
        index_memory_.end(),
        IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
            server_info_list_.begin(),
            server_info_list_.end(),
            [] (const ServerInfo& a, const ServerInfo& b) -> bool {
                return a.server_memory < b.server_memory;
        })
    );
    // for (int i = 0; i < index_memory_.size(); ++i) {
    //     std::cout << "No. " << i << " memory is " << server_info_list_[index_memory_[i]].server_memory << std::endl;
    // }
}

void ServerDataManager::BuildIndexServerLambda() {
    std::sort(
        index_server_lambda_.begin(),
        index_server_lambda_.end(),
        IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
            server_info_list_.begin(),
            server_info_list_.end(),
            [] (const ServerInfo& a, const ServerInfo& b) -> bool {
                return a.server_lambda < b.server_lambda;
        })
    );
    // for (int i = 0; i < index_server_lambda_.size(); ++i) {
    //     std::cout << "No. " << i << " server lambda is " << server_info_list_[index_server_lambda_[i]].server_lambda << std::endl;
    // }
}

ServerInfo& ServerDataManager::GetServerInfo(uint16_t n) {
    return server_info_list_[n];
}

std::pair<uint16_t, ServerInfo> ServerDataManager::GetServerNthBruteForce(uint16_t n) {
    return std::make_pair(index_brute_force_[n], server_info_list_[index_brute_force_[n]]);
}

// ServerInfo& ServerDataManager::GetServerNthPurchaseCostCpu(uint16_t n) {
//     return server_info_list_[index_purchase_cost_cpu_[n]];
// }

ServerInfo& ServerDataManager::GetServerNthCpu(uint16_t n) {
    return server_info_list_[index_cpu_[n]];
}

ServerInfo& ServerDataManager::GetServerNthMemory(uint16_t n) {
    return server_info_list_[index_memory_[n]];
}

std::pair<uint16_t, ServerInfo> ServerDataManager::GetServerLambdaMatch(float lambda, bool fresh_start) {
    uint16_t prev = prev_lambda_match_;
    if (fresh_start) {
        prev = 0;
    }
    uint16_t last = prev_lambda_match_ + 1;
    float curr_lambda = 0;
    
    // std::cout << "Binary search 1\n";
    while (last < num_servers_) {
        curr_lambda = server_info_list_[index_server_lambda_[last]].server_lambda;
        if (lambda <= curr_lambda) {
            break;
        }
        last += last - prev;
        // std::cout << "last is " << last << std::endl;
    }

    // std::cout << "Binary search 2\n";
    if (lambda == curr_lambda) return std::pair<uint16_t, ServerInfo>(index_server_lambda_[last], server_info_list_[index_server_lambda_[last]]);
    if (last >= num_servers_) last = num_servers_ - 1;
    while (last - prev > 1) {
        uint16_t mid = (prev + last) / 2;
        curr_lambda = server_info_list_[index_server_lambda_[mid]].server_lambda;
        // std::cout << "curr_lambda is " << curr_lambda << std::endl;
        // std::cout << "lambda is " << lambda << std::endl;
        // std::cout << "last is " << last << std::endl;
        // std::cout << "prev is " << prev << std::endl;
        if (curr_lambda > lambda) {
            last = mid;
        } else if (curr_lambda < lambda) {
            prev = mid;
        } else {
            return std::pair<uint16_t, ServerInfo>(index_server_lambda_[mid], server_info_list_[index_server_lambda_[mid]]);
        }
    }

    curr_lambda = server_info_list_[index_server_lambda_[prev]].server_lambda;
    // std::cout << "curr_lambda is " << curr_lambda << std::endl;
    // std::cout << "lambda is " << lambda << std::endl;
    // std::cout << "Binary search 3\n";
    if (prev == 0) {
        // std::cout << "next_lambda is " << server_info_list_[index_server_lambda_[prev + 1]].server_lambda << std::endl;
        if (prev + 1 < num_servers_ && std::abs(curr_lambda - lambda) > std::abs(server_info_list_[index_server_lambda_[prev + 1]].server_lambda - lambda)) {
            return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev + 1], server_info_list_[index_server_lambda_[prev + 1]]);
        } else {
            return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev], server_info_list_[index_server_lambda_[prev]]);
        }
    }

    // curr_lambda = server_info_list_[index_server_lambda_[last]].server_lambda;
    if (prev == num_servers_ - 1) {
        // std::cout << "prev_lambda is " << server_info_list_[index_server_lambda_[prev - 1]].server_lambda << std::endl;
        if (prev - 1 >= 0 && std::abs(curr_lambda - lambda) > std::abs(server_info_list_[index_server_lambda_[prev - 1]].server_lambda - lambda)) {
            return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev - 1], server_info_list_[index_server_lambda_[prev - 1]]);
        } else {
            return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev], server_info_list_[index_server_lambda_[prev]]);
        }
    }

    float curr_lambda_diff = std::abs(server_info_list_[index_server_lambda_[prev]].server_lambda - curr_lambda);
    float prev_lambda_diff = std::abs(server_info_list_[index_server_lambda_[prev - 1]].server_lambda - lambda);
    float next_lambda_diff = std::abs(server_info_list_[index_server_lambda_[prev + 1]].server_lambda - lambda);
    // std::cout << "next_lambda is " << server_info_list_[index_server_lambda_[prev + 1]].server_lambda << std::endl;
    // std::cout << "prev_lambda is " << server_info_list_[index_server_lambda_[prev - 1]].server_lambda << std::endl;
    if (next_lambda_diff >= curr_lambda_diff && next_lambda_diff >= prev_lambda_diff) {
        return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev + 1], server_info_list_[index_server_lambda_[prev + 1]]);
    } else if (curr_lambda_diff >= prev_lambda_diff && curr_lambda_diff >= next_lambda_diff) {
        return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev], server_info_list_[index_server_lambda_[prev]]);
    } else {
        return std::pair<uint16_t, ServerInfo>(index_server_lambda_[prev - 1], server_info_list_[index_server_lambda_[prev - 1]]);
    }
}
