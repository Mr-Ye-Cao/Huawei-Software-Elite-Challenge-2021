// @Author: Yu Xin

#include "server_selector.h"
#include "vm_data_manager.h"
#include "input_reader.h"

#include "vm_data_manager.h"
#include "input_reader.h"

ServerSelector::ServerSelector() :
  server_data_manager_(ServerDataManager::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()),
  vm_manager_(VmManager::GetInstance()) {

}

// 1. Sort the servers according to some criteria
// 2. Calculate the upper bound of the cost when using only the top server (muiti knapsack where value of each vm = size of each vm)
// 3. Remove one server from the back of the list
// 4. Repeat 2-3 with the next to the top server, by adding the next to the top server to the back of the list.
// 5. Repeat 2-4 until all server combo have been tried
void ServerSelector::MakeServerSelection() {
    std::vector<uint16_t> server_list;
    // MakeServerSelectionHelper(0, server_list);
    
}

int ServerSelector::WorseCaseSelectionVm(const uint16_t& id, const uint16_t& worst_num) {
    // Input:
    //      VMId(single or double):
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
    VmInfo& vinfo = vm_data_manager_.GetVm(id);

    bool is_single = vinfo.is_single;
    int16_t cpu_nu = vinfo.vm_cpu; 
    int16_t mem_nu = vinfo.vm_memory;

    cpu_nu *= worst_num;
    mem_nu *= worst_num;

    double lambda =(double)cpu_nu / mem_nu;

    ServerInfo& servin = server_data_manager_.GetServerLambdaMatch(lambda);

    int16_t scpu_nu = servin.server_cpu;
    int16_t smem_nu = servin.server_memory;
    int s_id = servin.server_id; // TODO(yuxin): add way to get server ID

    server_data_manager_.vm_to_server_[id] = s_id;

    int16_t num_server_buy;
    if(is_single){
        // greedily put vm into the server poin
        int spcpu_nu = scpu_nu / 2;
        int spmem_nu = smem_nu / 2;

        num_server_buy = std::ceil(std::max(std::ceil(spcpu_nu / cpu_nu), std::ceil(spmem_nu / mem_nu)) / 2);
    }else{
        //
        num_server_buy = std::max(std::ceil(scpu_nu / cpu_nu), std::ceil(smem_nu / mem_nu));
    }

    return num_server_buy;
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
