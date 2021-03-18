// @Author: Yu Xin

#include "vm_data_manager.h"

#include "index_comparator.h"

// TODO(Yu Xin): to be implemented.
VmDataManager::VmDataManager() :
  input_reader_(InputReader::GetInstance()),
  num_vm_(input_reader_.GetN()),
  vm_info_list_(input_reader_.GetVmInfoList()) {
    index_vm_name_.reserve(vm_info_list_.size());
    for (uint16_t i = 0; i < vm_info_list_.size(); ++i) {
        index_vm_name_[vm_info_list_[i].vm_name] = i;
    }
    // BuildIndexVmMemory();
}

uint16_t VmDataManager::GetNumVm() {
 return num_vm_;
}

VmInfo& VmDataManager::GetVm(const uint16_t& id) {
    return vm_info_list_[id];
}

VmInfo& VmDataManager::GetVm(const std::string& vm_name) {
    return vm_info_list_[index_vm_name_[vm_name]];
}

int VmDataManager::GetVmId(const std::string& vm_name) {
    return index_vm_name_[vm_name];
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
