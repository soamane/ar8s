#include "user_data.hpp"

UserData::UserData() : chatId(0), input(std::make_unique<UserInput>()) { }

UserData::UserData(int64_t chatId) : chatId(chatId), input(std::make_unique<UserInput>()), executor(std::make_unique<ExecutorStatus>()) { }

bool UserData::isInitialized() {
    return input->status->attackTimeEntered || input->status->phoneEntered;
}
