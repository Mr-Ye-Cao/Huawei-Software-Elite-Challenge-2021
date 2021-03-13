// @Author: Yu Xin

#include "request_data_manager.h"

// TODO(Yu Xin): to be implemented.
RequestDataManager::RequestDataManager() :
  input_reader_(InputReader::GetInstance()),
  daily_request_info_list_(input_reader_.GetDailyRequestInfoList()) {
    
}
