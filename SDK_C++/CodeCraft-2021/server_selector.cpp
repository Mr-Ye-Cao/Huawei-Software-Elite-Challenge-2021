//

#include "server_selector.h"

ServerSelector::ServerSelector() :
  server_data_manager_(ServerDataManager::GetInstance()) {

}

// 1. Sort the servers according to some criteria
// 2. Calculate the upper bound of the cost
// 3. Add the top server to the list
// 4. Add vm's until it can fit no more (knapsack where value of each vm = size of each vm)
// 5. Repeat 2-4 until all tasks have been added
void ServerSelector::MakeServerSelection() {
    
}
