// // @Author: Ye Cao  Zhongyan Wang

// #pragma once

// #include <vector>

// #include "request_data_manager.h"
// #include "server_data_manager.h"

// struct Server {
//     // (TODO) a vector containing the Vm tasks 

//     int16_t server_cpu_A; // The cpu number at node A
//     int16_t server_cpu_B; // The cpu number at node B

//     int16_t server_mem_A; // The memory number at node A
//     int16_t server_mem_B; // The memory number at node A

//     int16_t id; // Dynamic Id
// };

// class ServerManager {
//   public:
//     // Meyer singleton
//     static ServerManager& GetInstance();
//     ServerManager(const ServerManager&) = delete;
//     ServerManager& operator=(const ServerManager&) = delete;
    
//     std::vector<Server> server_cluster; 

//   private:
//     ServerManager();
//     ~ServerManager() = default;

// };
