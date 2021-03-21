// @Author: Team Eureka!

#include <iostream>
#include <string>

#include "request_data_manager.h"
#include "server_data_manager.h"
#include "vm_data_manager.h"
#include "vm_manager.h"
#include "server_selector.h"



int main() {
	// TODO:read standard input
	// file locatinon is relative to the location of this file
	// std::string file_location = "./test.txt";
	// InputReader input_reader();
	ServerDataManager& server_data_manager_(ServerDataManager::GetInstance());
	VmDataManager& vm_data_manager_(VmDataManager::GetInstance());
	RequestDataManager& request_data_manager_(RequestDataManager::GetInstance());
	VmManager& vm_manager_(VmManager::GetInstance());
	// TODO: process
	std::cout<<"Debug1"<<std::endl;
	ServerSelector& server_selector_(ServerSelector::GetInstance());
	std::cout<<"Debug2"<<std::endl;
    server_selector_.MakeServerSelection();

	// std::unordered_map<uint16_t, VmStatusWorstCaseInfo>& worst_vm_list = vm_manager_.GetWorstCaseVmList();
		std::cout<<"Debug3"<<std::endl;

	// std::unordered_map<std::uint16_t, std::uint16_t> server_purchase_chart;
	std::cout<<"Debug4"<<std::endl;

	// uint16_t total_server_num = 0;
 	// for(uint16_t index = 0; index < vm_data_manager_.GetNumVm(); index++){
	// 	 	std::cout<<"Debug5"<<std::endl;

	// 	VmStatusWorstCaseInfo& specifc_vm_worst = worst_vm_list[index];
	// 	// the number of specialized server to buy to contain this type ith vm
	// 	std::pair<uint16_t,uint16_t> spi = sv_select_.WorseCaseSelectionVm(index, specifc_vm_worst.vm_schedule_list.size());
	// 	uint16_t server_id = spi.first;
	// 	uint16_t server_number = spi.second;
	// 	total_server_num += server_number;
	// 	server_purchase_chart[server_id] += server_number;
	// }

	// std::cout<<"(purchase, "<<total_server_num<<")"<<std::endl;
	// std::unordered_map<uint16_t,uint16_t>::iterator it = server_purchase_chart.begin();
	// Iterate over the map using iterator
	for (const auto& server : server_selector_.GetServerPurchaseChart()) {
		std::cout<<"(" << server_data_manager_.GetServerInfo(server.first).server_name << ", " << server.second << ")" <<std::endl;
	}

	// TODO:write standard output
	// TODO:fflush(stdout);

	return 0;
}
