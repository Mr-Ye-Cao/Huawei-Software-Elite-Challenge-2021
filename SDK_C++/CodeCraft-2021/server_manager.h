// @Author: Ye Cao  Zhongyan Wang

#pragma once

#include <unordered_map>
#include <vector>

#include "request_data_manager.h"
#include "server_data_manager.h"
#include "vm_data_manager.h"

struct VmRequest {
    uint16_t vm_id; // static id, represents the type of vm
    uint16_t vm_unique_id; // Represents which of the vm's of the type is this
};

struct VmDeploymentInfo {
    uint16_t server_dynamic_id;
    bool is_A;
};


struct PurchasedServer {
    uint16_t server_dynamic_id;
    int16_t server_cpu_A; // The cpu number at node A
    int16_t server_cpu_B; // The cpu number at node B
    int16_t server_mem_A; // The memory number at node A
    int16_t server_mem_B; // The memory number at node B
    std::vector<VmRequest> vm_node_A; // vm's running on node A
    std::vector<VmRequest> vm_node_B; // vm's running on node B
};

class ServerManager {
  public:
    // Meyer singleton
    static ServerManager& GetInstance();
    ServerManager(const ServerManager&) = delete;
    ServerManager& operator=(const ServerManager&) = delete;

    void PurchaseServer(const uint16_t server_static_id, const uint16_t server_dynamic_id);
    int AddVmToServerBestFit(const uint16_t server_static_id, const uint16_t vm_id, const uint16_t vm_unique_id); // Adds to the most fit server of this type
    VmDeploymentInfo& GetVmDeploymentInfo(int32_t vm_unique_key);

  private:
    ServerManager();
    ~ServerManager() = default;
    inline bool Fits(const uint16_t& server_static_id, const uint16_t& server_dynamic_id, const uint16_t& vm_id, int32_t& cpu_left, int32_t& memory_left, const bool node_A=true);

    ServerDataManager& server_data_manager_;
    VmDataManager& vm_data_manager_;
    std::vector<std::vector<PurchasedServer> > server_cluster_; // server_id (static) to its status
    std::unordered_map<int32_t, VmDeploymentInfo> vm_unique_key_to_deployment_info_;
};
