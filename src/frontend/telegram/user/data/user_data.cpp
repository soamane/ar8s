#include "user_data.hpp"

UserData::UserData() : chatId(0), input(std::make_unique<UserInput>()) { }

UserData::UserData(int64_t chatId) : chatId(chatId), input(std::make_unique<UserInput>()) { }