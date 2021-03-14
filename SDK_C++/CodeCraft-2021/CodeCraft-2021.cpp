// @Author: Team Eureka!

#include <iostream>
#include <string>

#include "request_data_manager.h"
#include "server_data_manager.h"
#include "vm_data_manager.h"
#include "vm_manager.h"

int main() {
	// TODO:read standard input
	// file locatinon is relative to the location of this file
	// std::string file_location = "./test.txt";
	// InputReader input_reader();
	ServerDataManager& server_data_manager_(ServerDataManager::GetInstance());
	VmDataManager& vm_data_manager_(VmDataManager::GetInstance());
	RequestDataManager& request_data_manager_(RequestDataManager::GetInstance());
	VmManager& vm_manager_(VmManager::GetInstance());


	// TODO:process
	// TODO:write standard output
	// TODO:fflush(stdout);

	return 0;
}
