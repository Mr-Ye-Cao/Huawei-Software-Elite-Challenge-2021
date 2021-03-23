// @Author: Yu Xin

#pragma once

#include <string>

class OutputWriter {
  public:
    // Meyer singleton
    static OutputWriter& GetInstance();
    OutputWriter(const OutputWriter&) = delete;
    OutputWriter& operator=(const OutputWriter&) = delete;

    void OutputServerPurchaseHeader(const uint16_t& num);
    void OutputMigrationHeader(const uint32_t& num);
    void OutputSingleServerPurchase(const std::string& server_name, const uint16_t& num);
    void OutputSingleMigration(const uint16_t& request_id, const uint16_t& dest_server_id, const std::string& node = "");
    void OutputSingleVmDeployment(const uint16_t& server_id, const std::string& node = "");
    
  private:
    OutputWriter() = default;
    ~OutputWriter() = default;
};
