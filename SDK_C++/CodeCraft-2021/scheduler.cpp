// @Author: Yu Xin

#include "scheduler.h"

// TODO(Yu Xin): to be implemented.
Scheduler::Scheduler() :
  request_data_manager_(RequestDataManager::GetInstance()),
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
        OneIterationInternal();
        ++today_;
    }
}

void Scheduler::OneIterationInternal() {
    output_writer_.OutputServerPurchaseHeader(server_selector_.GetNumNewPurchases());
    server_selector_.OutputAllServerPurchases();
    output_writer_.OutputMigrationHeader(num_new_migrations_);
    vm_manager_.OutputTodayDeployment(today_);
}
