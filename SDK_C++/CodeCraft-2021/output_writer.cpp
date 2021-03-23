// @Author: Ye Cao

#include "output_writer.h"

#include <iostream>

OutputWriter& OutputWriter::GetInstance() {
    static OutputWriter output_writer;
    return output_writer;
}

void OutputWriter::OutputServerPurchaseHeader(const uint16_t& num) {
    std::cout << "(purchase, " << num << ")" << std::endl;
}

void OutputWriter::OutputMigrationHeader(const uint32_t& num) {
    std::cout << "(migration, " << num << ")" << std::endl;
}

void OutputWriter::OutputSingleServerPurchase(const std::string& server_name, const uint16_t& num) {
    std::cout << "(" << server_name << ", " << num << ")" << std::endl;
}

void OutputWriter::OutputSingleMigration(const uint16_t& request_id, const uint16_t& dest_server_id, const std::string& node) {
    std::cout << "(" << request_id << ", " << dest_server_id << ", " << node << ")" << std::endl;
}

void OutputWriter::OutputSingleVmDeployment(const uint16_t& server_id, const std::string& node) {
    std::cout << "(" << server_id << ", " << node << ")" << std::endl;
}
