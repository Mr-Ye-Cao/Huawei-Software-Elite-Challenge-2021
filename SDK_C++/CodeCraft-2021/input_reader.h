// @Author: Yu Xin

#pragma once

#include <vector>
#include <string>

namespace {
const char kSpace[] = " ";
const char kComma[] = ", ";
const char kZero[] = "0";
const char kAdd = 'a';
} // namespace

// server_name, #cpu, #memory, #purchase_cost, #running_cost
struct ServerInfo {
    int16_t server_cpu; // range [1, 1024] positive number
    int16_t server_memory; // range [1, 1024] positive number
    int16_t running_cost; // range [1, 5000]  positive number
    int32_t purchase_cost; // range [1, 5*1e5] positive number
    std::string server_name; // string length is at most 20 characters (number&alphabet)
};

// vm_name, #cpu, #memory, #is_single
struct VmInfo {
    int16_t vm_cpu; // range [1, 1024] positive number
    int16_t vm_memory; // range [1, 1024] positive number
    bool is_single; // single node or double node
    std::string vm_name; // string length is at most 20 characters (number&alphabet&.)
};

enum class RequestType {
  kAdd,
  kDelete
};

// requested_vm_name, (add, name, id) or (del, id)
struct RequestInfo {
    RequestType request_type;
    int32_t id; // id is 32bit integer BUT NOT NECESSARILY POSITIVE
    std::string requested_vm_name; // string length is at most 20 characters (number&alphabet&.)
};

struct DailyRequestInfo {
    int32_t number;
    std::vector<RequestInfo> request_info_list_;
};

class InputReader {
  public:
    InputReader(std::string file_location);
    ~InputReader() = default;
    
    void ReadInputFile();
    void ReadInputCin();

  private:
    const std::string file_location_;
    // Temporarily use vector as data structures for holding these information
    // TODO(Yu Xin): adapt to data structures if needed,
    // or just build indices over some keys later on for faster access.
    std::vector<ServerInfo> server_info_list_;
    std::vector<VmInfo> vm_info_list_;
    std::vector<DailyRequestInfo> daily_request_info_list_;
    
};
