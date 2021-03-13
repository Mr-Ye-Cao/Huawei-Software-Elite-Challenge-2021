// @Author: Ye Cao

#include "input_reader.h"

#include <iostream>
#include <cstring>
#include <fstream>

InputReader::InputReader(std::string file_location) 
  : file_location_(file_location) {
    ReadInputFile();
}

void InputReader::ReadInputFile(){
    std::ifstream file(file_location_);
    if(!file.is_open()) {
        std::cout << file.is_open() << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return;
    }
    unsigned short int N,M,T;
    std::string line;

    std::getline(file,line);
    N = std::stoi(line);
    for(int i = 0;i < N; ++i) {
        std::getline(file,line);
        line = line.substr(1, line.length() - 2);

        // (name, #cpu, #memory, #h_cost, #m_cost)
        std::string name;                // string length is at most 20 characters (number&alphabet)
        unsigned short cpu, memory; // range [1, 1024] positive number
        unsigned int h_cost; // range [1, 5*1e5] positive number
        unsigned short m_cost; // range [1, 5000]  positive number

        unsigned short start = 0;
        name = line.substr(0, (start = line.find(kSpace)) - 1);
        std::cout << "Server Name: " << name << std::endl;
        cpu = std::stoi(line.substr(start+1, start = line.find(kSpace, start + 1)));
        memory = std::stoi(line.substr(start + 1, start = line.find(kSpace, start + 1)));
        h_cost = std::stoi(line.substr(start + 1, start = line.find(kSpace, start + 1)));
        m_cost = std::stoi(line.substr(start + 1, start = line.find(kSpace, start + 1)));
    }

    std::getline(file, line);
    M = std::stoi(line);
    for(int i=0;i<M;i++) {
        std::getline(file,line);
        line=line.substr(1, line.length() - 2);

        // (name, #cpu, #memory, #SD)
        std::string name;                // string length is at most 20 characters (number&alphabet&.)
        unsigned short cpu, memory; // range [1, 1024] positive number (can be contained in at least one server)
        bool is_single;

        unsigned short start = 0;
        name = line.substr(0, (start = line.find(kSpace))-1);
        std::cout << "VM Name: " << name << std::endl;
        cpu = std::stoi(line.substr(start + 1, start = line.find(kSpace,start + 1)));
        memory = std::stoi(line.substr(start + 1, start = line.find(kSpace,start + 1)));
        is_single = line.substr(start + 1, start = line.find(kSpace,start + 1)) == "0" ? true : false;
    }

    std::getline(file,line);
    T = std::stoi(line);
    for(int i = 0; i < T; ++i) {
        std::getline(file,line);
        unsigned short R = std::stoi(line);     // the total requests over all days < 1e5
        for(int j=0;j<R;j++) {
            std::getline(file,line);
            line = line.substr(1, line.length() - 2);

            // (add, name, id) / (del, id)
            if(line[0] == 'a') {
                // add request
                std::string name;
                int id;       // ID is 32bit integer BUT NOT NECESSARILY POSITIVE

                int start = line.find(kSpace) + 1;
                name = line.substr(5, line.find_last_of(", ") - 6);
                std::cout << "Request: " << name << std::endl;
                id = std::stoi(line.substr(line.find_last_of(", ") + 1));                    
            } else {
                // delete requst (the vm with this id is guarenteed to exist)
                int id = std::stoi(line.substr(line.find_last_of(", ") + 1));       // id is 32bit integer BUT NOT NECESSARILY POSITIVE
            }
        }
    }
}

void InputReader::ReadInputCin(){

}
