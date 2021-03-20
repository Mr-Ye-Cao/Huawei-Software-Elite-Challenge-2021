// @Author: Yu Xin

#pragma once

#include <unordered_map>
#include <vector>

#include "input_reader.h"

class VmDataManager {
  public:
    // Meyer singleton
    static VmDataManager& GetInstance();
    VmDataManager(const VmDataManager&) = delete;
    VmDataManager& operator=(const VmDataManager&) = delete;

    uint16_t GetNumVm();
    VmInfo& GetVm(const uint16_t& id);
    VmInfo& GetVm(const std::string& vm_name);
    int GetVmId(const std::string& vm_name);
    VmInfo& GetVmNthLambda(const uint16_t n); // Lambda is CPU / Memory

  private:
    VmDataManager();
    ~VmDataManager() = default;
    InputReader& input_reader_;
    uint16_t num_vm_;
    std::vector<VmInfo>& vm_info_list_;
    std::unordered_map<std::string, int> index_vm_name_;
    std::vector<int> index_vm_lambda_;

    void BuildIndexVmLambda();
    
};
