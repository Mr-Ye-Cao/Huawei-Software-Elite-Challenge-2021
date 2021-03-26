// @Author: Ye Cao

#include "scheduler.h"

#include "server_manager.h"
#include "server_manager.h"

#include "vm_manager.h"

#include <iostream>

// TODO(Yu Xin): to be implemented.
Scheduler::Scheduler() :
  request_data_manager_(RequestDataManager::GetInstance()),
  server_data_manager_(ServerDataManager::GetInstance()),
  server_manager_(ServerManager::GetInstance()),
  server_selector_(ServerSelector::GetInstance()),
  vm_data_manager_(VmDataManager::GetInstance()),
  vm_manager_(VmManager::GetInstance()),
  output_writer_(OutputWriter::GetInstance()) {
    // server_selector_.MakeServerSelection();
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
    server_selector_.MakeServerSelectionSimple(today_);
    OutputEverythingToday();
}

void Scheduler::OutputEverythingToday() {
    server_selector_.OutputTodayServerPurchases();
    output_writer_.OutputMigrationHeader(0);
    server_manager_.OutputTodayDeployment(today_);
}
