// @Author: Yu Xin

#include "server_manager.h"

#include <limits>
#include <iostream>

ServerManager::ServerManager() : 
  request_data_manager_(RequestDataManager::GetInstance()),
  server_data_manager_(ServerDataManager::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()),
  vm_manager_(VmManager::GetInstance()),
  output_writer_(OutputWriter::GetInstance()) {
    server_cluster_.resize(server_data_manager_.GetNumServers());
    // server_cluster_today_.resize(server_data_manager_.GetNumServers());
}

ServerManager& ServerManager::GetInstance() {
    static ServerManager server_manager;
    return server_manager;
}

VmDeploymentInfo& ServerManager::GetVmDeploymentInfo(int32_t vm_unique_key) {
    return vm_unique_key_to_deployment_info_[vm_unique_key];
}

void ServerManager::PurchaseServer(const uint16_t server_static_id, const uint16_t server_dynamic_id) {
    PurchasedServer server;
    ServerInfo& server_info = server_data_manager_.GetServerInfo(server_static_id);
    server.server_dynamic_id = server_dynamic_id;
    server.server_cpu_A = server_info.server_cpu / 2;
    server.server_cpu_B = server.server_cpu_A;
    server.server_mem_A = server_info.server_memory / 2;
    server.server_mem_B = server.server_mem_A;
    server_cluster_[server_static_id].push_back(server);
}

// void ServerManager::ApplyTodayPurchase() {
//     for (int server_id = 0; server_id < server_cluster_today_.size(); ++server_id) {
//         std::vector<PurchasedServer>& server_list = server_cluster_today_[server_id];
//         for (const PurchasedServer& server : server_list) {
//             server_cluster_[server_id].push_back(server);
//         }
//         server_list.clear();
//     }
// }

int ServerManager::AddVmToServerBestFit(const uint16_t server_static_id, const uint16_t vm_id, const uint16_t vm_unique_id) {
    if (server_cluster_[server_static_id].size() == 0) return -1;
    uint16_t best_fit = 0; // currently use cpu to find best fit
    bool best_fit_node_A;
    VmInfo& vm_info = vm_data_manager_.GetVm(vm_id);
    int32_t min_curr_cpu_left = std::numeric_limits<int32_t>::max();
    int32_t min_curr_memory_left = std::numeric_limits<int32_t>::max();
    int32_t curr_cpu_left;
    int32_t curr_memory_left;
    bool has_a_fit = false;
    if (vm_info.is_single) {
        for (uint16_t i = 0; i < server_cluster_[server_static_id].size(); ++i) {
            bool node_A = true;
            for (int i = 0; i < 2; ++i) {
                if (Fits(server_static_id, i, vm_id, curr_cpu_left, curr_memory_left, node_A)) {
                    has_a_fit = true;
                    if (curr_cpu_left < min_curr_cpu_left) {
                        min_curr_cpu_left = curr_cpu_left;
                        best_fit = i;
                        best_fit_node_A = node_A;
                    }
                    if (curr_memory_left < min_curr_memory_left) {
                        min_curr_memory_left = curr_memory_left;
                    }
                }
                node_A = false;
            }
        }
        if (has_a_fit) {
            PurchasedServer& best_fit_server = server_cluster_[server_static_id][best_fit];
            VmRequest vm_request;
            VmDeploymentInfo& vm_deployment_info = vm_unique_key_to_deployment_info_[vm_unique_id];
            vm_request.vm_id = vm_id;
            vm_request.vm_unique_id = vm_unique_id;
            vm_deployment_info.vm_id = vm_id;
            vm_deployment_info.server_dynamic_id = best_fit_server.server_dynamic_id;
            best_fit_server.vm_list.push_back(std::make_pair(vm_id, vm_unique_id));
            // vm_deployment_info.request_id = vm_manager_.GetRequestOnVm(vm_id, vm_unique_id, )
            if (best_fit_node_A) {
                best_fit_server.server_cpu_A -= vm_info.vm_cpu;
                best_fit_server.server_mem_A -= vm_info.vm_memory;
                best_fit_server.vm_node_A.push_back(vm_request);
                vm_deployment_info.is_A = true;
                // std::cout << "single node VM " << vm_data_manager_.GetVm(vm_id).vm_name
                //     << " on server " << best_fit_server.server_dynamic_id << " node A of type " << server_data_manager_.GetServerInfo(server_static_id).server_name << std::endl;
            } else {
                best_fit_server.server_cpu_B -= vm_info.vm_cpu;
                best_fit_server.server_mem_B -= vm_info.vm_memory;
                best_fit_server.vm_node_B.push_back(vm_request);
                vm_deployment_info.is_A = false;
                // std::cout << "single node request " << vm_data_manager_.GetVm(vm_id).vm_name
                //     << " on server " << best_fit_server.server_dynamic_id << " node B of type " << server_data_manager_.GetServerInfo(server_static_id).server_name << std::endl;
            }
            return 0;
        }
    } else {
        for (uint16_t i = 0; i < server_cluster_[server_static_id].size(); ++i) {
            if (Fits(server_static_id, i, vm_id, curr_cpu_left, curr_memory_left)) {
                has_a_fit = true;
                if (curr_cpu_left < min_curr_cpu_left) {
                    min_curr_cpu_left = curr_cpu_left;
                    best_fit = i;
                }
                if (curr_memory_left < min_curr_memory_left) {
                    min_curr_memory_left = curr_memory_left;
                }
            }
        }
        if (has_a_fit) {
            PurchasedServer& best_fit_server = server_cluster_[server_static_id][best_fit];
            best_fit_server.server_cpu_A -= vm_info.vm_cpu / 2;
            best_fit_server.server_mem_A -= vm_info.vm_memory / 2;
            best_fit_server.server_cpu_B -= vm_info.vm_cpu / 2;
            best_fit_server.server_mem_B -= vm_info.vm_memory / 2;
            VmRequest vm_request;
            vm_request.vm_id = vm_id;
            vm_request.vm_unique_id = vm_unique_id;
            best_fit_server.vm_node_A.push_back(vm_request);
            best_fit_server.vm_node_B.push_back(vm_request);
            vm_unique_key_to_deployment_info_[vm_unique_id].server_dynamic_id = best_fit_server.server_dynamic_id;
            // std::cout << "double node request " << vm_data_manager_.GetVm(vm_id).vm_name << " on server " << best_fit_server.server_dynamic_id << " of type " << server_data_manager_.GetServerInfo(server_static_id).server_name << std::endl;
            return 0;
        }
    }
    return -1;
}

bool ServerManager::Fits(const uint16_t& server_static_id, const uint16_t& server_dynamic_id,
                         const uint16_t& vm_id, int32_t& cpu_left, int32_t& memory_left, const bool node_A) {
    VmInfo& vm_info = vm_data_manager_.GetVm(vm_id);
    PurchasedServer& curr_server = server_cluster_[server_static_id][server_dynamic_id];
    // if (node_A) return false;
    if (vm_info.is_single) {
        if (node_A) {
            // std::cout << "curr_server.server_cpu_A: " << curr_server.server_cpu_A << std::endl;
            // std::cout << "vm_info.vm_cpu: " << vm_info.vm_cpu << std::endl;
            // std::cout << "curr_server.server_mem_A: " << curr_server.server_mem_A << std::endl;
            // std::cout << "vm_info.vm_memory: " << vm_info.vm_memory << std::endl;
            if (curr_server.server_cpu_A >= vm_info.vm_cpu && curr_server.server_mem_A >= vm_info.vm_memory) {
                cpu_left = curr_server.server_cpu_A;
                memory_left = curr_server.server_mem_A;
                return true;
            } else {
                return false;
            }
        } else {
            // std::cout << "curr_server.server_cpu_B: " << curr_server.server_cpu_B << std::endl;
            // std::cout << "vm_info.vm_cpu: " << vm_info.vm_cpu << std::endl;
            // std::cout << "curr_server.server_mem_B: " << curr_server.server_mem_B << std::endl;
            // std::cout << "vm_info.vm_memory: " << vm_info.vm_memory << std::endl;
            if (curr_server.server_cpu_B >= vm_info.vm_cpu && curr_server.server_mem_B >= vm_info.vm_memory) {
                cpu_left = curr_server.server_cpu_B;
                memory_left = curr_server.server_mem_B;
                return true;
            } else {
                return false;
            }
        }
    } else {
        int16_t vm_cpu_per_node = vm_info.vm_cpu / 2;
        int16_t vm_memory_per_node = vm_info.vm_memory / 2;
        // std::cout << "curr_server.server_cpu_A: " << curr_server.server_cpu_A << std::endl;
        // std::cout << "curr_server.server_cpu_B: " << curr_server.server_cpu_B << std::endl;
        // std::cout << "vm_cpu_per_node: " << vm_cpu_per_node << std::endl;
        // std::cout << "curr_server.server_mem_A: " << curr_server.server_mem_A << std::endl;
        // std::cout << "curr_server.server_mem_B: " << curr_server.server_mem_B << std::endl;
        // std::cout << "vm_memory_per_node: " << vm_memory_per_node << std::endl;
        if (curr_server.server_cpu_A >= vm_cpu_per_node && curr_server.server_mem_A >= vm_memory_per_node && 
            curr_server.server_cpu_B >= vm_cpu_per_node && curr_server.server_mem_B >= vm_memory_per_node) {
            cpu_left = curr_server.server_cpu_A + curr_server.server_cpu_B;
            memory_left = curr_server.server_mem_A + curr_server.server_mem_B;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void ServerManager::OutputTodayDeployment(const uint16_t& day) {
    for (const int32_t& idx : request_data_manager_.GetRequestOfDay(day)) {
        const uint16_t curr_vm_id = request_data_manager_.GetVmId(idx);
        VmDeploymentInfo& curr_info = GetVmDeploymentInfo(request_data_manager_.GetUniqueVmId(idx));
        if (vm_data_manager_.GetVm(curr_vm_id).is_single) {
            const std::string node = curr_info.is_A ? "A" : "B";
            output_writer_.OutputSingleVmDeployment(idx, curr_info.server_dynamic_id, node);
        } else {
            output_writer_.OutputSingleVmDeployment(idx, curr_info.server_dynamic_id);
        }
    }
}
