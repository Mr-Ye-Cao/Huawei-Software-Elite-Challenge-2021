// @Author: Yu Xin

#include "server_selector.h"

#include <iostream>

#include "vm_data_manager.h"
#include "input_reader.h"

ServerSelector::ServerSelector() :
  server_data_manager_(ServerDataManager::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()),
  vm_manager_(VmManager::GetInstance()) {

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
	uint16_t total_server_num = 0;
 	for(uint16_t index = 0; index < vm_data_manager_.GetNumVm(); index++){
        std::cout<<"Debug5"<<std::endl;

		VmStatusWorstCaseInfo& specifc_vm_worst = vm_manager_.GetWorstCaseVmList()[index];
		// the number of specialized server to buy to contain this type ith vm
		std::pair<uint16_t,uint16_t> spi = WorseCaseSelectionVm(index, specifc_vm_worst.vm_schedule_list.size());
		uint16_t server_id = spi.first;
		uint16_t server_number = spi.second;
		total_server_num += server_number;
		server_purchase_chart_[server_id] += server_number;
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
    VmInfo& vinfo = vm_data_manager_.GetVmNthLambda(id);

    bool is_single = vinfo.is_single;
    int16_t cpu_nu = vinfo.vm_cpu; 
    int16_t mem_nu = vinfo.vm_memory;

    cpu_nu *= worst_num;
    mem_nu *= worst_num;

    float lambda = (float)cpu_nu / mem_nu;

    std::pair<uint16_t, ServerInfo> servin = server_data_manager_.GetServerLambdaMatch(lambda);

    const uint16_t& s_id = servin.first;
    const uint16_t& scpu_nu = servin.second.server_cpu;
    const uint16_t& smem_nu = servin.second.server_memory;

    vm_manager_.vm_to_server_[id] = s_id;

    uint16_t num_server_buy;
    if (is_single) {
        // greedily put vm into the server poin
        int spcpu_nu = scpu_nu / 2;
        int spmem_nu = smem_nu / 2;

        num_server_buy = std::ceil(std::max(std::ceil((float)spcpu_nu / cpu_nu), std::ceil((float)spmem_nu / mem_nu)) / 2);
    } else {
        //
        num_server_buy = std::ceil(std::max(std::ceil((float)scpu_nu / cpu_nu), std::ceil((float)smem_nu / mem_nu)));
    }

    return std::make_pair(s_id, num_server_buy);
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
