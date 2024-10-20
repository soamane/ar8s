#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include "../input/user_input.hpp"
#include "../status/input/input_status.hpp"
#include "../status/executor/executor_status.hpp"

struct UserData {
    UserInput input;
    InputStatus inputStatus;
    ExecutorStatus executorStatus;

    int64_t chatId;

    void clear();
};

#endif // !USER_DATA_HPP