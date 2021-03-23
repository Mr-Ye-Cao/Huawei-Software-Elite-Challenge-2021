// @Author: Yu Xin

#include "server_selector.h"

#include <iostream>

#include "input_reader.h"
#include "utils.h"
#include "vm_data_manager.h"

ServerSelector::ServerSelector() :
  server_data_manager_(ServerDataManager::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()),
  vm_manager_(VmManager::GetInstance()),
  server_manager_(ServerManager::GetInstance()),
  output_writer_(OutputWriter::GetInstance()) {

}

ServerSelector& ServerSelector::GetInstance() {
    static ServerSelector server_selector;
    return server_selector;
}

std::unordered_map<std::uint16_t, std::uint16_t>& ServerSelector::GetServerPurchaseChart() {
    return server_purchase_chart_;
}

// 1. Sort the servers according to some criteria
// 2. Calculate the upper bound of the cost when using only the top server (muiti knapsack where value of each vm = size of each vm)
// 3. Remove one server from the back of the list
// 4. Repeat 2-3 with the next to the top server, by adding the next to the top server to the back of the list.
// 5. Repeat 2-4 until all server combo have been tried
void ServerSelector::MakeServerSelection() {
    std::vector<uint16_t> server_list;
 	for(uint16_t index = 0; index < vm_data_manager_.GetNumVm(); index++){
        // std::cout<<"Debug5"<<std::endl;

		VmStatusWorstCaseInfo& specifc_vm_worst = vm_manager_.GetWorstCaseVmList()[index];
		// the number of specialized server to buy to contain this type ith vm
		std::pair<uint16_t,uint16_t> spi = WorseCaseSelectionVm(index, specifc_vm_worst.vm_schedule_list.size());
		uint16_t server_id = spi.first;
		uint16_t server_number = spi.second;
		total_server_num_ += server_number;
		server_purchase_chart_[server_id] += server_number;
        PurchaseServers(server_id, server_number);

	}
}

void ServerSelector::PurchaseServers(uint16_t server_id, uint16_t num) {
    for (uint16_t i = 0; i < num; ++i) {
        server_manager_.PurchaseServer(server_id, server_dynamic_id_);
        ++server_dynamic_id_;
    }
}

void ServerSelector::AddVmsToServers(uint16_t server_id, uint16_t vm_id, uint16_t num) {
    for (uint16_t i = 0; i < num; ++i) {
        if (server_manager_.AddVmToServerBestFit(server_id, vm_id, i) != 0) {
            server_manager_.PurchaseServer(server_id, server_dynamic_id_);
            std::cout << "Forced to buy a server" << std::endl;
            ++server_dynamic_id_;
            server_manager_.AddVmToServerBestFit(server_id, vm_id, i);
        }
    }
}

std::pair<int16_t,int16_t> ServerSelector::WorseCaseSelectionVm(const uint16_t& id, const uint16_t& worst_num) {
    // Input:
    //      VMId:
    //      WorstCaseNum:

    /**
     * We assume that based on the type of VM(single/double) the server
     * will be specialized to the specific type of VM.
     * 
     * The VMInfor will enable us to calculate the lambda value from which
     * we can choose the closest server to contain the specific vm
     * 
     * Lastly, after finding out which server to use, we can
     * calculate the worsecasenum of servers to contain the worst case vm. 
     **/

    // get the vm id
    VmInfo& vm_info = vm_data_manager_.GetVmNthLambda(id);
    bool is_single = vm_info.is_single;
    int16_t vm_cpu = vm_info.vm_cpu; 
    int16_t vm_memory = vm_info.vm_memory;
    float lambda = vm_info.vm_lambda;

    std::pair<uint16_t, ServerInfo> server_info = server_data_manager_.GetServerLambdaMatch(lambda);
    const uint16_t& server_id = server_info.first;
    int16_t num_server_buy;
    // std::cout << "VM node cpu " << vm_cpu << std::endl;
    // std::cout << "VM node memory " << vm_memory << std::endl;
    if (is_single) {
        // greedily put vm into the server poin
        int16_t server_node_cpu = server_info.second.server_cpu / 2;
        int16_t server_node_memory = server_info.second.server_memory / 2;
        int16_t vm_cpu_per_node = utils::DivideCeil(server_node_cpu, vm_cpu);
        int16_t vm_memory_per_node = utils::DivideCeil(server_node_memory, vm_memory);
        // std::cout << "Server node remaining cpu load " << server_node_cpu % vm_cpu << std::endl;
        // std::cout << "Server node remaining memory load " << server_node_memory % vm_memory << std::endl;
        num_server_buy = utils::DivideCeil(utils::DivideCeil((int)worst_num, (int)std::min(vm_cpu_per_node, vm_memory_per_node)), 2);
    } else {
        int16_t vm_cpu_per_server = utils::DivideCeil(server_info.second.server_cpu, vm_cpu);
        int16_t vm_memory_per_server = utils::DivideCeil(server_info.second.server_memory, vm_memory);
        // std::cout << "Server remaining cpu load " << vm_cpu_per_server % vm_cpu << std::endl;
        // std::cout << "Server remaining memory load " << vm_memory_per_server % vm_memory << std::endl;
        num_server_buy = utils::DivideCeil((int)worst_num, (int)std::min(vm_cpu_per_server, vm_memory_per_server));
    }

    return std::make_pair(server_id, num_server_buy);
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

void ServerSelector::PrintAllServerPurchases() {
    output_writer_.OutputServerPurchaseHeader(total_server_num_);
    for (const auto& server : server_purchase_chart_) {
        output_writer_.OutputSingleServerPurchase(server_data_manager_.GetServerInfo(server.first).server_name, server.second);
    }
}
