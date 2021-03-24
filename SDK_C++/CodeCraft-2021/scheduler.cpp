// @Author: Yu Xin

#include "scheduler.h"

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
        OneIterationInternal();
        ++today_;
    }
}

void Scheduler::OneIterationInternal() {
    uint16_t num_new_purchases = server_selector_.GetNumNewPurchases();
    output_writer_.OutputServerPurchaseHeader(num_new_purchases);
    if (num_new_purchases > 0) {
        server_selector_.OutputAllServerPurchases();
    }
    output_writer_.OutputMigrationHeader(num_new_migrations_);
    server_manager_.OutputTodayDeployment(today_);
    server_selector_.ResetNumNewPurchases();
}
