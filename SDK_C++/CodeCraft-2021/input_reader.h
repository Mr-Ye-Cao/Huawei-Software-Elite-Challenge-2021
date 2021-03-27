// @Author: Yu Xin

#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace {
const std::string kFileLocation = "./test.txt";
const char kSpace[] = " ";
const char kComma[] = ", ";
const char kZero[] = "0";
const char kAdd = 'a';
} // namespace

// server_name, #cpu, #memory, #purchase_cost, #running_cost
struct ServerInfo {
    int16_t server_cpu; // range [1, 1024] positive number
    int16_t server_memory; // range [1, 1024] positive number
    // int16_t running_cost; // range [1, 5000]  positive number
    // int32_t purchase_cost; // range [1, 5*1e5] positive number
    float server_lambda; // lambda is CPU / Memory
    std::string server_name; // string length is at most 20 characters (number&alphabet)
};

// vm_name, #cpu, #memory, #is_single
struct VmInfo {
    int16_t vm_cpu; // range [1, 1024] positive number
    int16_t vm_memory; // range [1, 1024] positive number
    float vm_lambda; // lambda is CPU / Memory
    bool is_single; // single node or double node
    std::string vm_name; // string length is at most 20 characters (number&alphabet&.)
};

// enum class RequestType {
//   kAdd,
//   kDelete
// };

// requested_vm_name, (add, name, request_id) or (del, request_id)
struct RequestInfo {
    int16_t start_day; // inclusive
    int16_t end_day; // non inclusive
    uint16_t vm_id; // vm id that the task is assigned to
    // int32_t vm_num; // the num of vm of vm_id type that this task is assigned to
    int32_t unique_vm_id; // the num of vm of vm_id type that this task is assigned to
    // int32_t request_id; // request_id is 32bit integer BUT NOT NECESSARILY POSITIVE
    std::string requested_vm_name; // string length is at most 20 characters (number&alphabet&.)
};

// struct DailyRequestInfo {
//     int32_t number;
//     std::vector<RequestInfo> request_info_list_;
// };

class InputReader {
  public:
    // Meyer singleton
    static InputReader& GetInstance();
    InputReader(const InputReader&) = delete;
    InputReader& operator=(const InputReader&) = delete;

    uint16_t GetN();
    std::vector<ServerInfo>& GetServerInfoList();
    uint16_t GetM();
    std::vector<VmInfo>& GetVmInfoList();
    uint16_t GetT();
    std::unordered_map<int32_t, RequestInfo>& GetRequestInfoList();
    

  private:
    InputReader();
    ~InputReader() = default;
    void ReadInputFile();
    void ReadInputCin();
    // Temporarily use vector as data structures for holding these information
    // TODO(Yu Xin): adapt to data structures if needed,
    // or just build indices over some keys later on for faster access.
    uint16_t N; // Number of servers
    std::vector<ServerInfo> server_info_list_;
    uint16_t M; // Number of VM's
    std::vector<VmInfo> vm_info_list_;
    uint16_t T; // Number of days
    std::unordered_map<int32_t, RequestInfo> request_info_list_; // request_id is 32bit integer BUT NOT NECESSARILY POSITIVE
    
};
