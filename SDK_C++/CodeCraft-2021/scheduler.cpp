// @Author: Ye Cao

#include "scheduler.h"

#include "server_manager.h"
#include "server_manager.h"

#include "vm_manager.h"

#include <iostream>

// TODO(Yu Xin): to be implemented.
Scheduler::Scheduler() :
  request_data_manager_(RequestDataManager::GetInstance()),
  server_manager_(ServerManager::GetInstance()),
  server_selector_(ServerSelector::GetInstance()),
  vm_manager_(VmManager::GetInstance()),
  output_writer_(OutputWriter::GetInstance()) {
    server_selector_.MakeServerSelection();
}

Scheduler& Scheduler::GetInstance() {
    static Scheduler scheduler;
    return scheduler;
}

void Scheduler::Run() {
    for (uint16_t day = 0; day < request_data_manager_.GetDays(); ++day) {
        // std::cout<<"Debug0"<<std::endl;

        OneIterationInternal();
        ++today_;
    }
}

void Scheduler::OneIterationInternal() {
  // choose a server type in this case we only buy the first server
  uint16_t sta_server_id = 0;

  // get the task requests of vms for today
  std::vector<std::pair<uint16_t, int32_t> > today_tasks =  vm_manager_.GetNewVmOfToday(today_);

  // keep track of the dynamic server id
  uint16_t dyn_server_id = 0;

//   std::cout<<"Debug1"<<std::endl;

  // for each task put the vm into the server
    for (int i = 0; i < today_tasks.size(); ++i){
        if (server_manager_.AddVmToServerBestFit(sta_server_id, today_tasks[i].first , today_tasks[i].second) == -1) {
            // std::cout<<"Debug2"<<std::endl;

            // we need to purchase a new server for the vm
            server_manager_.PurchaseServer(sta_server_id, dyn_server_id);

            // update the dynamic server id
            ++dyn_server_id;
            
            if (server_manager_.AddVmToServerBestFit(sta_server_id, today_tasks[i].first , today_tasks[i].second) == -1){
                std::cout<< "This server type is unable to finish the tasks.\n";
                break;
            } else {
                // the vm is successfully put into server
                // do nothing in this case
            }

            // reset the purchase
            // server_selector_.ResetNumNewPurchases();
        } else {
            // the vm is successfully put into server
            // do nothing in this case
            // std::cout<<"Debug3"<<std::endl;
        }
    }
    server_selector_.OutputTodayServerPurchases();
    server_manager_.OutputTodayDeployment(today_);
}
