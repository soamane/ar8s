#include "executor_status.hpp"

void ExecutorStatus::clear() {
    this->attackInProgress = false;
    this->attackStopped = false;
}