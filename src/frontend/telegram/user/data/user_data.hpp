#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include "../input/user_input.hpp"

#include <memory>
#include <cstdint>

struct UserData {
    UserData();
    UserData(int64_t chatId);

    int64_t chatId;
    std::unique_ptr<UserInput> input;

    bool isInitialized();
};

#endif // !USER_DATA_HPP