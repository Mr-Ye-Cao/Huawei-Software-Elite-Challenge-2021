// @Author: Ye Cao

#include "input_reader.h"

#include <iostream>
#include <cstring>
#include <fstream>

InputReader::InputReader() {
    ReadInputFile();
}

void InputReader::ReadInputFile(){
    std::ifstream file(kFileLocation);
    if(!file.is_open()) {
        std::cout << file.is_open() << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return;
    }
    std::string line;

    // (name, #cpu, #memory, #purchase_cost, #running_cost)
    uint16_t server_start = 0;
    std::getline(file,line);
    N = std::stoi(line);
    server_info_list_.resize(N);
    for(int i = 0;i < N; ++i) {
        std::getline(file,line);
        line = line.substr(1, line.length() - 2);
        server_start = 0;
        
        ServerInfo& curr = server_info_list_[i];
        // TODO(yuxin): section where things start to go wrong (substr start out of range error)
        curr.server_name = line.substr(0, (server_start = line.find(kSpace)) - 1);
        // std::cout << "Server Name: " << curr.server_name << std::endl;
        curr.server_cpu = std::stoi(line.substr(server_start + 1, server_start = line.find(kSpace, server_start + 1)));
        curr.server_memory = std::stoi(line.substr(server_start + 1, server_start = line.find(kSpace, server_start + 1)));
        curr.purchase_cost = std::stoi(line.substr(server_start + 1, server_start = line.find(kSpace, server_start + 1)));
        curr.running_cost = std::stoi(line.substr(server_start + 1, server_start = line.find(kSpace, server_start + 1)));
        curr.server_lambda = (float)curr.server_cpu / curr.server_memory;
    }

    // (name, #cpu, #memory, #SD)
    std::string vm_name; // string length is at most 20 characters (number&alphabet&.)
    uint16_t vm_cpu, vm_memory; // range [1, 1024] positive number (can be contained in at least one server)
    bool is_single;
    uint16_t vm_start = 0;
    std::getline(file, line);
    M = std::stoi(line);
    vm_info_list_.resize(M);
    for(int i = 0; i < M; ++i) {
        std::getline(file,line);
        line = line.substr(1, line.length() - 2);
        vm_start = 0;

        VmInfo& curr = vm_info_list_[i];
        curr.vm_name = line.substr(0, (vm_start = line.find(kSpace))-1);
        // std::cout << "VM Name: " << curr.vm_name << std::endl;
        curr.vm_cpu = std::stoi(line.substr(vm_start + 1, vm_start = line.find(kSpace, vm_start + 1)));
        curr.vm_memory = std::stoi(line.substr(vm_start + 1, vm_start = line.find(kSpace, vm_start + 1)));
        curr.is_single = line.substr(vm_start + 1, vm_start = line.find(kSpace, vm_start + 1)) == kZero ? true : false;
        curr.vm_lambda = (float)curr.vm_cpu / curr.vm_memory;
    }

    // Adds requests
    std::getline(file,line);
    T = std::stoi(line);
    // daily_request_info_list_.resize(T);
    int request_start = 0;
    int last_comma_pos = 0;
    for(int i = 0; i < T; ++i) {
        std::getline(file,line);
        uint16_t R = std::stoi(line);     // the total requests over all days < 1e5
        // RequestInfo& curr_day_request = daily_request_info_list_[i];
        // curr_day_request.number = R;
        // curr_day_request.request_info_list_.resize(R);
        for(int j = 0; j < R; ++j) {
            std::getline(file,line);
            line = line.substr(1, line.length() - 2);

            // (add, name, request_id) / (del, request_id)
            last_comma_pos = line.find_last_of(kComma);
            if(line[0] == kAdd) {
                request_start = line.find(kSpace) + 1;
                const std::string& requested_vm_name = line.substr(5, last_comma_pos - 6);
                const int32_t request_id = std::stoi(line.substr(last_comma_pos + 1, line.size() - last_comma_pos - 1));
                // std::cout << "Requests to add: " << request_id << " on day " << i << std::endl;
                // RequestInfo& curr_request = request_info_list_[request_id];
                request_info_list_[request_id].requested_vm_name = requested_vm_name;
                request_info_list_[request_id].start_day = i;
                request_info_list_[request_id].end_day = T - 1; // Default end day is the end of all days
            } else {
                // delete request (the vm with this request_id is guarenteed to exist)
                const int32_t& request_id = std::stoi(line.substr(last_comma_pos + 1, line.size() - last_comma_pos - 1));
                request_info_list_[request_id].end_day = i;
                // std::cout << "Requests to delete ID: " << request_id << " on day " << i << std::endl;
            }
        }
    }
}

void InputReader::ReadInputCin(){

}

InputReader& InputReader::GetInstance() {
    static InputReader input_reader;
    return input_reader;
}

uint16_t InputReader::GetN() { return N; }
std::vector<ServerInfo>& InputReader::GetServerInfoList() { return server_info_list_; }
uint16_t InputReader::GetM() { return M; }
std::vector<VmInfo>& InputReader::GetVmInfoList() { return vm_info_list_; }
uint16_t InputReader::GetT() { return T; }
std::unordered_map<int32_t, RequestInfo>& InputReader::GetRequestInfoList() { return request_info_list_; }
