// @Author: Yu Xin

#pragma once

#include <vector>

#include "input_reader.h"

class VmDataManager {
  public:
    VmDataManager();
    ~VmDataManager() = default;
  private:
    InputReader& input_reader_;
    std::vector<VmInfo>& vm_info_list_;
    
};
