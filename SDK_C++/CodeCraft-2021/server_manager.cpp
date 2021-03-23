// @Author: Yu Xin

#include "server_manager.h"

ServerManager::ServerManager() : 
  server_data_manager_(ServerDataManager::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()) {
    server_cluster_.resize(server_data_manager_.GetNumServers());
}

ServerManager& ServerManager::GetInstance() {
    static ServerManager server_manager;
    return server_manager;
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

int ServerManager::AddVmToServerBestFit(const uint16_t server_static_id, const uint16_t vm_id, const uint16_t vm_unique_id) {
    if (server_cluster_[server_static_id].size() == 0) return -1;
    uint16_t best_fit = 0; // currently use cpu to find best fit
    bool best_fit_node_A; // currently use cpu to find best fit
    VmInfo& vm_info = vm_data_manager_.GetVm(vm_id);
    int32_t min_curr_cpu_left = std::numeric_limits<int32_t>::max();
    int32_t min_curr_memory_left = std::numeric_limits<int32_t>::max();
    int32_t curr_cpu_left;
    int32_t curr_memory_left;
    bool has_a_fit = false;
    if (vm_info.is_single) {
        for (uint16_t i = 0; i < server_cluster_[server_static_id].size(); ++i) {
            for (bool node_A = true; node_A; node_A = !node_A) {
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
            }
        }
        if (has_a_fit) {
            PurchasedServer& best_fit_server = server_cluster_[server_static_id][best_fit];
            VmRequest vm_request;
            vm_request.vm_id = vm_id;
            vm_request.vm_unique_id = vm_unique_id;
            if (best_fit_node_A) {
                best_fit_server.server_cpu_A -= vm_info.vm_cpu;
                best_fit_server.server_mem_A -= vm_info.vm_memory;
                best_fit_server.vm_node_A.push_back(vm_request);
            } else {
                best_fit_server.server_cpu_B -= vm_info.vm_cpu;
                best_fit_server.server_mem_B -= vm_info.vm_memory;
                best_fit_server.vm_node_B.push_back(vm_request);
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
            return 0;
        }
    }
    return -1;
}

bool ServerManager::Fits(const uint16_t& server_static_id, const uint16_t& server_dynamic_id,
                         const uint16_t& vm_id, int32_t& cpu_left, int32_t& memory_left, const bool node_A) {
    VmInfo& vm_info = vm_data_manager_.GetVm(vm_id);
    PurchasedServer& curr_server = server_cluster_[server_static_id][server_dynamic_id];
    if (vm_info.is_single) {
        if (node_A) {
            if (curr_server.server_cpu_A >= vm_info.vm_cpu && curr_server.server_mem_A >= vm_info.vm_memory) {
                cpu_left = curr_server.server_cpu_A;
                memory_left = curr_server.server_mem_A;
                return true;
            } else {
                return false;
            }
        } else {
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
