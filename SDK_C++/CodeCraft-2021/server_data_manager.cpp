 // @Author: Yu Xin

#include "server_data_manager.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

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

    // index_brute_force_.resize(server_info_list_.size());
    // index_purchase_cost_cpu_.resize(server_info_list_.size());
    index_cpu_.resize(server_info_list_.size());
    index_memory_.resize(server_info_list_.size());
    num_servers_ = server_info_list_.size();
    index_server_lambda_.resize(server_info_list_.size());
    for (uint16_t i = 0; i < num_servers_; ++i) {
        // index_brute_force_[i] = i;
        // index_purchase_cost_cpu_[i] = i;
        index_cpu_[i] = i;
        index_memory_[i] = i;
        index_server_lambda_[i] = i;
    }
    // BuildIndexBruteForce();
    // BuildIndexPurchaseCostCpu();
    BuildIndexCpu();
    BuildIndexMemory();
    BuildIndexServerLambda();
    BuildServerLambdaIntervalMatch();
    // std::cout << "Server data manager constructed" << std::endl;
}

// void ServerDataManager::BuildIndexBruteForce(/*bool (*le)(const ServerInfo&, const ServerInfo&)*/) {
//     std::sort(
//         index_brute_force_.begin(),
//         index_brute_force_.end(),
//         IndexComparator<std::vector<ServerInfo>::const_iterator, ServerInfo>(
//             server_info_list_.begin(),
//             server_info_list_.end(),
//             [] (const ServerInfo& a, const ServerInfo& b) -> bool {
//                 return a.purchase_cost < b.purchase_cost; // TODO: change this to your sorting criteria
//         })
//     );
//     // for (int i = 0; i < index_brute_force_.size(); ++i) {
//     //     std::cout << "No. " << i << " is " << server_info_list_[index_brute_force_[i]].purchase_cost << std::endl;
//     // }
// }

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

// std::pair<uint16_t, ServerInfo> ServerDataManager::GetServerNthBruteForce(uint16_t n) {
//     return std::make_pair(index_brute_force_[n], server_info_list_[index_brute_force_[n]]);
// }

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

void ServerDataManager::BuildServerLambdaIntervalMatch() {
    server_lambda_match_.resize(kNumIntervals);
    float least_lambda = server_info_list_[*index_server_lambda_.begin()].server_lambda;
    float most_lambda = server_info_list_[index_server_lambda_[num_servers_ - 1]].server_lambda;
    float increment_le_1 = (1 - least_lambda) / (kNumIntervals / 2);
    float increment_gt_1 = (most_lambda - 1) / (kNumIntervals / 2);
    uint8_t interval_num = 0;
    uint16_t curr_best_server_id = 0;
    uint16_t curr_best_server_cpu_memory_sum = 0;
    float lambda_offset = -1;
    uint16_t interval_offset = -1;
    for (uint16_t i = 0; i < num_servers_; ++i) {
        const ServerInfo& curr_server_info = server_info_list_[index_server_lambda_[i]];
        uint16_t curr_cpu_memory_sum = curr_server_info.server_cpu + curr_server_info.server_memory;
        float curr_server_lambda = curr_server_info.server_lambda;
        if (curr_server_lambda <= 1) {
            if (curr_server_lambda >= least_lambda + interval_num * increment_le_1 && curr_server_lambda < least_lambda + (interval_num + 1) * increment_le_1) {

            } else {
                ServerOfLambdaInterval& curr_interval = server_lambda_match_[interval_num];
                curr_interval.lambda_start = least_lambda + interval_num * increment_le_1;
                curr_interval.lambda_end = least_lambda + (interval_num + 1) * increment_le_1;
                curr_interval.server_id = curr_best_server_id;
                curr_best_server_cpu_memory_sum = 0;
                ++interval_num;
            }
        } else {
            if (lambda_offset < 0) {
                lambda_offset = least_lambda + interval_num * increment_le_1;
                interval_offset = interval_num;
                interval_num = 0;
            }
            if (curr_server_lambda >= lambda_offset + interval_num * increment_gt_1 && curr_server_lambda < interval_num + (interval_num + 1) * increment_gt_1) {

            } else {
                ServerOfLambdaInterval& curr_interval = server_lambda_match_[interval_offset + interval_num];
                curr_interval.lambda_start = lambda_offset + interval_num * increment_gt_1;
                curr_interval.lambda_end = lambda_offset + (interval_num + 1) * increment_gt_1;
                curr_interval.server_id = curr_best_server_id;
                curr_best_server_cpu_memory_sum = 0;
                ++interval_num;
            }
        }
        if (curr_cpu_memory_sum > curr_best_server_cpu_memory_sum) {
            curr_best_server_id = index_server_lambda_[i];
            curr_best_server_cpu_memory_sum = curr_cpu_memory_sum;
        }
    }
    // for (const ServerOfLambdaInterval& interval_ite : server_lambda_match_) {
    //     std::cout << interval_ite.lambda_start << ", " << interval_ite.lambda_end << ", " << server_info_list_[interval_ite.server_id].server_cpu << ", " << server_info_list_[interval_ite.server_id].server_memory << std::endl;
    // }
}

std::pair<uint16_t, ServerInfo> ServerDataManager::GetServerLambdaIntervalMatch(float lambda) {
    uint16_t server;
    for (const ServerOfLambdaInterval& interval_ite : server_lambda_match_) {
        server = interval_ite.server_id;
        if (lambda < interval_ite.lambda_end) break;
    }
    return std::make_pair(server, server_info_list_[server]);
}

uint16_t ServerDataManager::GetBestServerThatFits(int16_t vm_cpu, int16_t vm_memory, bool is_single) {
    if (!is_single) {
        vm_cpu /= 2;
        vm_memory /= 2;
    }
    // std::cout << "vm_cpu is " << vm_cpu << std::endl;
    // std::cout << "vm_memory is " << vm_memory << std::endl;
    std::vector<uint16_t>::iterator it_cpu = std::lower_bound(
        index_cpu_.begin(),
        index_cpu_.end(),
        vm_cpu,
        [this] (const uint16_t& a, const uint16_t& b) -> bool {
            return this->server_info_list_[a].server_cpu < b;
        }
    );
    std::vector<uint16_t>::iterator it_memory = std::lower_bound(
        index_memory_.begin(),
        index_memory_.end(),
        vm_memory,
        [this] (const uint16_t& a, const uint16_t& b) -> bool {
            return this->server_info_list_[a].server_memory < b;
        }
    );
    std::vector<uint16_t> server_list;
    std::unordered_set<uint16_t> cpu_index_set(it_cpu, index_cpu_.end());
    std::unordered_set<uint16_t> servers_that_fits_both;
    // for (auto& it = it_cpu; it != index_cpu_.end(); ++it) {
    //     // std::cout << *it << ", ";
    // }
    // std::cout << std::endl;
    for (auto& it = it_memory; it != index_memory_.end(); ++it) {
        // std::cout << *it << ", ";
        if (cpu_index_set.find(*it) != cpu_index_set.end()) {
            servers_that_fits_both.insert(*it);
        }
    }
    // std::cout << std::endl;
    // std::set_intersection(it_cpu, index_cpu_.end(),
    //                       it_memory, index_memory_.end(),
    //                       back_inserter(server_list));
    // Currently returns anyone in the list of server that can fit, 
    // but later we can apply some conditions for optimization.
    // std::cout << "servers_that_fits_both has size " << servers_that_fits_both.size() << std::endl;
    return *servers_that_fits_both.begin();
}
