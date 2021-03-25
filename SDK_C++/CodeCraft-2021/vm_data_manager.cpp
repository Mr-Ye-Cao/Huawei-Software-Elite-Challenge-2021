// @Author: Yu Xin

#include "vm_data_manager.h"

#include <algorithm>
#include <iostream>

#include "index_comparator.h"

// TODO(Yu Xin): to be implemented.
VmDataManager::VmDataManager() :
  input_reader_(InputReader::GetInstance()),
  num_vm_(input_reader_.GetM()),
  vm_info_list_(input_reader_.GetVmInfoList()) {
    index_vm_name_.reserve(vm_info_list_.size());
    index_vm_lambda_.resize(vm_info_list_.size());
    for (uint16_t i = 0; i < vm_info_list_.size(); ++i) {
        index_vm_name_[vm_info_list_[i].vm_name] = i;
        index_vm_lambda_[i] = i;
    }
    BuildIndexVmLambda();
    BuildIndexVmSinDou();
}

VmInfo& VmDataManager::GetVm(const uint16_t& id) {
    return vm_info_list_[id];
}

VmInfo& VmDataManager::GetVm(const std::string& vm_name) {
    return vm_info_list_[index_vm_name_[vm_name]];
}

uint16_t VmDataManager::GetVmId(const std::string& vm_name) {
    return index_vm_name_[vm_name];
}

VmInfo& VmDataManager::GetVmNthLambda(const uint16_t n) {
    return vm_info_list_[index_vm_lambda_[n]];
}

uint16_t& VmDataManager::GetVmIndexNthLambda(const uint16_t n) {
    return index_vm_lambda_[n];
}

VmInfo& VmDataManager::GetNthSingleVm(const uint16_t n) {
    // Assumes correct input
    return vm_info_list_[index_single_vm_[n]];
}

VmInfo& VmDataManager::GetNthDoubleVm(const uint16_t n) {
    // Assumes correct input
    return vm_info_list_[index_double_vm_[n]];
}

uint16_t VmDataManager::GetNumSingleVm(const uint16_t n) {
    return num_single_vm_;
}

uint16_t VmDataManager::GetNumDoubleVm(const uint16_t n) {
    return num_double_vm_;
}

void VmDataManager::BuildIndexVmLambda() {
    index_vm_lambda_.resize(vm_info_list_.size());
    for (uint16_t i = 0; i < vm_info_list_.size(); ++i) {
        index_vm_lambda_[i] = i;
    }
    std::sort(
        index_vm_lambda_.begin(),
        index_vm_lambda_.end(),
        IndexComparator<std::vector<VmInfo>::const_iterator, VmInfo>(
            vm_info_list_.begin(),
            vm_info_list_.end(),
            [] (const VmInfo& a, const VmInfo& b) -> bool {
                return a.vm_lambda < (float)b.vm_lambda;
        })
    );
    // for (int i = 0; i < index_vm_lambda_.size(); ++i) {
    //     std::cout << "No. " << i << " vm lambda is " << vm_info_list_[index_vm_lambda_[i]].vm_lambda << std::endl;
    // }
}

void VmDataManager::BuildIndexVmSinDou() {
    index_single_vm_.clear();
    index_double_vm_.clear();
    for (uint16_t i = 0; i < vm_info_list_.size(); ++i) {
        if (vm_info_list_[i].is_single) index_single_vm_.push_back(i);
        else index_double_vm_.push_back(i);
    }
    num_single_vm_ = index_single_vm_.size();
    num_double_vm_ = index_double_vm_.size();
}

// void VmDataManager::BuildIndexVmMemory() {
//     index_vm_memory_.resize(vm_info_list_.size());
//     for (uint16_t i = 0; i < vm_info_list_.size(); ++i) {
//         index_vm_memory_[i] = i;
//     }
//     std::sort(
//         index_vm_memory_.begin(),
//         index_vm_memory_.end(),
//         IndexComparator<std::vector<VmInfo>::const_iterator, VmInfo>(
//             vm_info_list_.begin(),
//             vm_info_list_.end(),
//             [] (const VmInfo& a, const VmInfo& b) -> bool {
//                 return a.vm_memory < b.vm_memory;
//         })
//     )
// }

VmDataManager& VmDataManager::GetInstance() {
    static VmDataManager vm_data_manager;
    return vm_data_manager;
}
