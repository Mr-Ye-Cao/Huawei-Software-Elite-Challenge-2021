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
    uint16_t GetNumSingleVm(const uint16_t n);
    uint16_t GetNumDoubleVm(const uint16_t n);
    VmInfo& GetNthSingleVm(const uint16_t n);
    VmInfo& GetNthDoubleVm(const uint16_t n);

  private:
    VmDataManager();
    ~VmDataManager() = default;
    InputReader& input_reader_;
    uint16_t num_vm_;
    uint16_t num_single_vm_;
    uint16_t num_double_vm_;
    std::vector<VmInfo>& vm_info_list_;
    std::unordered_map<std::string, uint16_t> index_vm_name_;
    std::vector<uint16_t> index_vm_lambda_;
    std::vector<uint16_t> index_single_vm_;
    std::vector<uint16_t> index_double_vm_;

    void BuildIndexVmLambda();
    void BuildIndexVmSinDou();
    
};
