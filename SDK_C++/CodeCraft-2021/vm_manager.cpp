// @Author: Yu Xin

#include <iostream>
#include <set>

#include "index_comparator.h"
#include "vm_manager.h"

VmManager::VmManager() :
  vm_data_manager_(VmDataManager::GetInstance()),
  request_data_manager_(RequestDataManager::GetInstance()),
//   server_manager_(ServerManager::GetInstance()),
  output_writer_(OutputWriter::GetInstance()),
  days_(request_data_manager_.GetDays()),
  num_vm_(vm_data_manager_.GetNumVm()),
  request_info_list_(request_data_manager_.GetRequestInfoList()),
  server_data_manager_(ServerDataManager::GetInstance()),
  prev_lambda_match_(server_data_manager_.GetNumServers() / 2) {
    vm_schedules_.reserve(num_vm_);
    // num_newly_added_tasks_.resize(days_);
    // std::cout << "initializing vm manager" << std::endl;
    for (auto& request : request_info_list_) {
        const int32_t request_id = request.first;
        RequestInfo& request_info = request.second;
        // ++num_newly_added_tasks_[request_info.start_day];
        uint16_t vm_id = vm_data_manager_.GetVmId(request_info.requested_vm_name);
        request_info.vm_id = vm_id;

        // std::cout << "Request " << request_id << " needs vm " << vm_id << " from day " << request_info.start_day << " to day " << request_info.end_day << std::endl;

        VmStatusInfo& curr_vm_status = vm_schedules_[vm_id];
        VmStatusWorstCaseInfo& curr_vm_status_worst_case = vm_schedules_worst_case_[vm_id];
        curr_vm_status.num_running.resize(days_);

        // For finding the lowest ranked vm that can run this request
        std::set<int32_t> possible_vms;
        for (const auto& vm : curr_vm_status_worst_case.vm_schedule_list) {
            possible_vms.insert(vm.first);
        }
        for (int16_t day = request_info.start_day; day <= request_info.end_day && !possible_vms.empty(); ++day) {
            for (auto it = possible_vms.begin(); it != possible_vms.end(); ) {
                if (curr_vm_status_worst_case.vm_schedule_list[*it][day].is_running) {
                    it = possible_vms.erase(it);
                } else {
                    ++it;
                }
            }
        }
        int32_t vm_scheduled_for_worst_case;
        if (possible_vms.empty()) {
            curr_vm_status_worst_case.vm_schedule_list[curr_unused_vm_id_] = std::vector<VmToday>(days_);
            // curr_vm_status_worst_case.vm_unique_id_map[curr_unused_vm_id_] = curr_vm_status_worst_case.vm_schedule_list.size() - 1;
            vm_scheduled_for_worst_case = curr_unused_vm_id_;
            ++curr_unused_vm_id_;
        } else {
            vm_scheduled_for_worst_case = *(possible_vms.begin());
        }
        request_info.unique_vm_id = vm_scheduled_for_worst_case;
        // auto it = std::find_if(curr_vm_status_worst_case.vm_unique_id_map.begin(), curr_vm_status_worst_case.vm_unique_id_map.end(),
        //                    [&vm_scheduled_for_worst_case] (const std::unordered_map<int32_t, int32_t>::value_type& p) { return p.second == vm_scheduled_for_worst_case; });

        // request_info.unique_vm_id = it->first;
        // std::cout << "vm_scheduled_for_worst_case is " << vm_scheduled_for_worst_case << std::endl;

        for (int16_t day = request_info.start_day; day <= request_info.end_day; ++day) {
            ++curr_vm_status.num_running[day];
            curr_vm_status.request_id_list[day].push_back(request_id);
            curr_vm_status_worst_case.vm_schedule_list[vm_scheduled_for_worst_case][day].is_running = true;
            curr_vm_status_worst_case.vm_schedule_list[vm_scheduled_for_worst_case][day].request_id = request_id;
            // std::cout << "Request " << curr_vm_status.request_id_list[day].back()
            //     << " added, we have " << curr_vm_status.num_running[day] << " "
            //     << request_info.requested_vm_name << " running on day " << day << std::endl;
        }
    }

    // Since we know how many of each vm are needed for the worst case,
    // we can calculate worst case core and memory requirements
    for (const auto& it : vm_schedules_worst_case_) {
        const uint16_t& vm_id = it.first;
        VmInfo& curr_vm = vm_data_manager_.GetVm(vm_id);
        const int64_t& num_requested = it.second.vm_schedule_list.size();
        worst_case_core_ += curr_vm.vm_cpu * num_requested;
        worst_case_memory_ += curr_vm.vm_memory * num_requested;
    }
    // std::cout << "worst_case_core_ is " << worst_case_core_ << std::endl;
    // std::cout << "worst_case_memory_ is " << worst_case_memory_ << std::endl;
}

VmManager& VmManager::GetInstance() {
    static VmManager vm_manager;
    return vm_manager;
}

std::unordered_map<uint16_t, VmStatusWorstCaseInfo>& VmManager::GetWorstCaseVmList() {
    return vm_schedules_worst_case_;
}

int32_t VmManager::GetRequestOnVm(uint16_t vm_id, int32_t vm_unique_id, uint16_t day) {
    return vm_schedules_worst_case_[vm_id].vm_schedule_list[vm_unique_id][day].request_id;
}

std::vector<std::pair<uint16_t, int32_t> > VmManager::GetNewVmOfToday(uint16_t day) {
    std::vector<std::pair<uint16_t, int32_t> > new_vm_of_today;
    if (day == 0) {
        for (const auto& vm_type_ite : vm_schedules_worst_case_) {
            const uint16_t vm_id = vm_type_ite.first;
            for (const auto& vm_ite : vm_type_ite.second.vm_schedule_list) {
                const int32_t vm_unique_id = vm_ite.first;
                if (vm_ite.second[day].is_running) {
                    new_vm_of_today.push_back(std::make_pair(vm_id, vm_unique_id));
                }
            }
        }
    } else {
        for (const auto& vm_type_ite : vm_schedules_worst_case_) {
            const uint16_t vm_id = vm_type_ite.first;
            for (const auto& vm_ite : vm_type_ite.second.vm_schedule_list) {
                const int32_t vm_unique_id = vm_ite.first;
                if (vm_ite.second[day].is_running && !vm_ite.second[day - 1].is_running) {
                    new_vm_of_today.push_back(std::make_pair(vm_id, vm_unique_id));
                }
            }
        }
    }
    return new_vm_of_today;
}
