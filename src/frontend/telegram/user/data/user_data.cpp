#include "user_data.hpp"

UserData::UserData() : chatId(0), input(std::make_unique<UserInput>()), executor(std::make_unique<ExecutorStatus>()) { }

UserData::UserData(int64_t chatId) : UserData() {
    this->chatId = chatId;
}

bool UserData::isInitialized() const {
    return input->status->attackTimeEntered || input->status->phoneEntered;
}
