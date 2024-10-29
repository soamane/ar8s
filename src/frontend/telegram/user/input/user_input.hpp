#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include "../status/input/input_status.hpp"

#include <memory>
#include <string>

struct UserInput {
    UserInput();

    int attackTime;
    std::string phone;

    std::unique_ptr<InputStatus> status;
};

#endif // !USER_INPUT_HPP