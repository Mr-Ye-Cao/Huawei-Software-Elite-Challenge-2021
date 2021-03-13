// @Author: Yu Xin

#include "vm_data_manager.h"

// TODO(Yu Xin): to be implemented.
VmDataManager::VmDataManager() :
  input_reader_(InputReader::GetInstance()),
  vm_info_list_(input_reader_.GetVmInfoList()) {
    
}
