// @Author: Yu Xin

#include "vm_manager.h"

VmManager::VmManager() :
  vm_data_manager_(VmDataManager::GetInstance()),
  request_data_manager_(RequestDataManager::GetInstance()) {
    
}
