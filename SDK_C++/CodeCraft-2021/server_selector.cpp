// @Author: Yu Xin

#include "server_selector.h"

ServerSelector::ServerSelector() :
  server_data_manager_(ServerDataManager::GetInstance()) {

}

// 1. Sort the servers according to some criteria
// 2. Calculate the upper bound of the cost when using only the top server (muiti knapsack where value of each vm = size of each vm)
// 3. Remove one server from the back of the list
// 4. Repeat 2-3 with the next to the top server, by adding the next to the top server to the back of the list.
// 5. Repeat 2-4 until all server combo have been tried
void ServerSelector::MakeServerSelection() {
    std::vector<uint16_t> server_list;
    MakeServerSelectionHelper(0, server_list);
}

void ServerSelector::MakeServerSelectionHelper(uint16_t curr_server_id, std::vector<uint16_t> server_list) {
    // 1. Sort the servers according to some criteria
    // Done in server_data_manager_
    for (uint16_t i = curr_server_id; i < server_data_manager_.GetNumServers(); ++i) {
        // 2. Calculate the upper bound of the cost when using only the top server (muiti knapsack where value of each vm = size of each vm)
        

        // 3. Remove one server from the back of the list

    }
    // 4. Repeat 2-3 with the next to the top server, by adding the next to the top server to the back of the list.
    // 5. Repeat 2-4 until all server combo have been tried
}
