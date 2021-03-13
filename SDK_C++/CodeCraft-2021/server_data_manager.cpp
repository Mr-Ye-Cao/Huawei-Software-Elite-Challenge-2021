// @Author: Yu Xin

#include "server_data_manager.h"

// TODO(Yu Xin): to be implemented.
ServerDataManager::ServerDataManager() :
  input_reader_(InputReader::GetInstance()),
  server_info_list_(input_reader_.GetServerInfoList()) {
    
}
